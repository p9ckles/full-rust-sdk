#include <Windows.h>

#include "utilities/driver.h"
#include "utilities/debug.h"
#include "globals.h"
#include "sdk/offset.h"
#include "sdk/datatypes/vector.h"
#include "sdk/datatypes/matrix.h"
#include "sdk/interfaces/decryption.h"
#include "overlay/overlay.h"

extern c_memory m;

class base_network {
public:
    uint64_t base;
    uint64_t wrapper_class_ptr;
    uint64_t wrapper_class;
    uint64_t parent_class;
    uint64_t parent_static_fields;
    uint64_t object_dictionary;
    uint64_t object_dictionary_array;
    uint64_t object_dictionary_size;

    bool update() {
        static uint64_t last_dbg = 0;
        bool do_log = (GetTickCount64() - last_dbg) > 3000;
        if (do_log) last_dbg = GetTickCount64();

        base = m.read<uint64_t>(globals->game_assembly + offsets::EntityList::base);
        if (do_log) dbg::log(base ? "OK" : "FAIL", "[NET] base=0x%016I64X  (GA+0x%I64X)", (unsigned long long)base, (unsigned long long)offsets::EntityList::base);

        if (!base) return false;

        // Step 1: static_fields = typeinfo + offsets::base_networkable::static_fields
        uint64_t static_fields_ptr = m.read<uint64_t>(base + offsets::base_networkable::static_fields);
        if (do_log) dbg::log(static_fields_ptr ? "OK" : "FAIL", "[NET] static_fields=0x%016I64X", (unsigned long long)static_fields_ptr);
        if (!static_fields_ptr) return false;

        // Step 2: ce_obj = static_fields + offsets::base_networkable::client_entities, decrypt -> client_entities
        // Your dump shows client_entities = 0x20 (older code used 0x18, which yields null).
        uint64_t ce_obj = m.read<uint64_t>(static_fields_ptr + offsets::base_networkable::client_entities);
        if (do_log) dbg::log(ce_obj ? "OK" : "FAIL", "[NET] ce_obj=0x%016I64X", (unsigned long long)ce_obj);
        if (!ce_obj) return false;

        uint64_t client_entities = decryption::BaseNetworkable::decrypt(ce_obj);
        if (do_log) dbg::log(client_entities ? "OK" : "FAIL", "[NET] client_entities=0x%016I64X", (unsigned long long)client_entities);
        if (!client_entities) return false;

        // Probe all candidate offsets in client_entities to find the one with highest count
        static bool ce_probed = false;
        if (do_log && !ce_probed) {
            ce_probed = true;
            for (int off = 0x10; off <= 0x40; off += 0x10) {
                uint64_t cand = m.read<uint64_t>(client_entities + off);
                if (!cand) continue;
                uint64_t el = decryption::BaseNetworkable::decrypt_list(cand);
                if (!el) continue;
                uint32_t cnt = m.read<uint32_t>(el + 0x14);
                dbg::log("INFO", "[NET] probe ce+0x%02X -> el=0x%016I64X  count=%u", off, (unsigned long long)el, cnt);
            }
        }

        // Step 3: el_obj = client_entities + offsets::base_networkable::entity_list, decrypt_list -> entity_list
        uint64_t el_obj = m.read<uint64_t>(client_entities + offsets::base_networkable::entity_list);
        if (do_log) dbg::log(el_obj ? "OK" : "FAIL", "[NET] el_obj=0x%016I64X", (unsigned long long)el_obj);
        if (!el_obj) return false;

        uint64_t entity_list = decryption::BaseNetworkable::decrypt_list(el_obj);
        if (do_log) dbg::log(entity_list ? "OK" : "FAIL", "[NET] entity_list=0x%016I64X", (unsigned long long)entity_list);
        if (!entity_list) return false;

        // Step 4: object_dictionary pointer inside entity_list (plain ptr, no decrypt)
        // Many dumps place this at +0x18; if it still fails, we can switch to offsets::base_networkable::object_dictionary.
        uint64_t object_dict = m.read<uint64_t>(entity_list + offsets::base_networkable::entLS);
        if (do_log) dbg::log(object_dict ? "OK" : "FAIL", "[NET] object_dict=0x%016I64X", (unsigned long long)object_dict);
        if (!object_dict) return false;

        uint64_t arr_obj = m.read<uint64_t>(object_dict + 0x10);
        uint64_t arr_len = m.read<uint64_t>(object_dict + 0x18);
        if (do_log) dbg::log(arr_obj ? "OK" : "FAIL", "[NET] arr=0x%016I64X  count=%llu", (unsigned long long)arr_obj, (unsigned long long)arr_len);

        object_dictionary = object_dict;
        object_dictionary_array = arr_obj;
        object_dictionary_size = (uint32_t)arr_len;
        if (do_log) dbg::log(object_dictionary_size > 0 ? "OK" : "FAIL", "[NET] dict_array=0x%016I64X  dict_size=%u", (unsigned long long)object_dictionary_array, object_dictionary_size);

        return object_dictionary_size > 0;
    }
};

class main_camera {
public:
    uint64_t base;
    uint64_t camera;

    matrix_4x4 view_matrix;

    vector3 camera_position;

    bool update() {
        static uint64_t last_dbg = 0;
        bool do_log = (GetTickCount64() - last_dbg) > 3000;
        if (do_log) last_dbg = GetTickCount64();

        base = m.read<uint64_t>(globals->game_assembly + offsets::Camera::base);
        if (do_log) dbg::log(base ? "OK" : "FAIL", "[CAM] base=0x%016I64X  (GA+0x%I64X)", (unsigned long long)base, (unsigned long long)offsets::Camera::base);

        if (!base) return false;

        uint64_t step1 = m.read<uint64_t>(base + offsets::Camera::staticfields);
        if (do_log) dbg::log(step1 ? "OK" : "FAIL", "[CAM] step1 +staticfields(0x%I64X)=0x%016I64X", (unsigned long long)offsets::Camera::staticfields, (unsigned long long)step1);

        // One-shot dump: scan step1 for non-zero pointers to find real wrapperclass offset
        static bool cam_dumped = false;
        if (step1 && !cam_dumped) {
            cam_dumped = true;
            dbg::log("INFO", "[CAM] dumping step1=0x%016I64X for non-zero ptrs:", (unsigned long long)step1);
            for (int off = 0; off <= 0xF0; off += 8) {
                uint64_t val = m.read<uint64_t>(step1 + off);
                if (val) dbg::log("INFO", "[CAM]   +0x%02X = 0x%016I64X", off, (unsigned long long)val);
            }
        }

        // instance is directly at offsets::main_camera::instance in static_fields
        uint64_t instance_ptr = m.read<uint64_t>(step1 + offsets::main_camera::instance);
        if (do_log) dbg::log(instance_ptr ? "OK" : "FAIL", "[CAM] instance +0x%I64X=0x%016I64X",
            (unsigned long long)offsets::main_camera::instance, (unsigned long long)instance_ptr);

        camera = m.read<uint64_t>(instance_ptr + offsets::Camera::parentstaticfields);
        if (do_log) dbg::log(camera ? "OK" : "FAIL", "[CAM] camera +parentstaticfields(0x%I64X)=0x%016I64X", (unsigned long long)offsets::Camera::parentstaticfields, (unsigned long long)camera);

        view_matrix = m.read<matrix_4x4>(camera + offsets::Camera::viewmatrix);
        camera_position = m.read<vector3>(camera + offsets::Camera::cameraposition);
        if (do_log) dbg::log("INFO", "[CAM] pos=(%.2f,%.2f,%.2f)", camera_position.x, camera_position.y, camera_position.z);

        return camera != 0;
    }

    vector2 world_to_screen(const vector3& world) {
        vector3 transpose_vector{ view_matrix._14, view_matrix._24, view_matrix._34 };
        vector3 right_vector{ view_matrix._11, view_matrix._21, view_matrix._31 };
        vector3 up_vector{ view_matrix._12, view_matrix._22, view_matrix._32 };

        float forward = transpose_vector.dot(world) + view_matrix._44;

        if (forward < 0.01f)
            return vector2(-1, -1);

        float side = right_vector.dot(world) + view_matrix._41;
        float up = up_vector.dot(world) + view_matrix._42;

        float first = side / forward;
        float second = up / forward;

        float width = static_cast<float>(GetSystemMetrics(SM_CXSCREEN));
        float height = static_cast<float>(GetSystemMetrics(SM_CYSCREEN));

        return vector2((width / 2.0f) * (1.0f + first), (height / 2.0f) * (1.0f - second));
    }
};

inline std::unique_ptr<main_camera> camera = std::make_unique<main_camera>();
inline std::unique_ptr<base_network> base_networkable = std::make_unique<base_network>();