#pragma once
#include <cstdint>

namespace offsets {

    namespace Il2cpp {
        inline uint64_t base = 0xE90AE60;  // updated: il2cpp::get_handle
    }

    namespace base_networkable {
        inline uint64_t typeinfo = 0xE5B9DF0;  // updated
        inline uint64_t static_fields = 0xB8;
        inline uint64_t client_entities = 0x20;  // updated: was 0x18
        inline uint64_t entity_list = 0x10;
        inline uint64_t buffer = 0x10;  // updated
        inline uint64_t entListBase = 0x10;
        inline uint64_t entLS = 0x18;
        inline uint64_t object_dictionary = 0x20;  // updated
        inline uint64_t entity_content = 0x10;
        inline uint64_t entity_size = 0x18;
        inline uint64_t prefabID = 0x30;
        inline uint64_t children = 0x40;
        inline uint64_t parent_entity = 0x70;  // fixed: was 0x10
    }

    namespace main_camera {
        inline uint64_t typeinfo = 0xE5CC948;  // updated
        inline uint64_t static_fields = 0xB8;
        inline uint64_t instance = 0x90;  // from dump: instance = 0x90
        inline uint64_t wrapper_class = 0x78;
        inline uint64_t parent_static_fields = 0x10;
        inline uint64_t buffer = 0x10;
        inline uint64_t viewMatrix = 0x2FC;
        inline uint64_t position = 0x444;
        inline uint64_t cullingMask = 0x43C;
    }

    namespace TodSky {
        inline uint64_t Class = 0xE5385F0;  // updated
        inline uint64_t static_fields = 0xB8;
        inline uint64_t instance = 0x60;  // updated
        inline uint64_t wrapper_class = 0x0;
        inline uint64_t parent_static_fields = 0x10;
        inline uint64_t TOD_AmbientParameters = 0x98;
        inline uint64_t TOD_LightParameters = 0x88;
        inline uint64_t TOD_FogParameters = 0x90;
        inline uint64_t TOD_CloudParameters = 0x80;
        inline uint64_t TOD_DayParameters = 0x58;
        inline uint64_t TOD_NightParameters = 0x60;
    }

    namespace TOD_LightParameters {
        inline uint64_t ambientColor = 0x48;
        inline uint64_t lightColor = 0x20;
        inline uint64_t rayColor = 0x28;
        inline uint64_t skyColor = 0x30;
        inline uint64_t cloudColor = 0x38;
    }

    namespace TOD_FogParameters {
        inline uint64_t fogColor = 0x40;
    }

    namespace TOD_NightParameters {
        inline uint64_t ambientMultiplier = 0x60;
    }

    namespace BaseEntity {
        inline uint64_t model = 0xF0;
        inline uint64_t Visible = 0x191;
    }

    namespace BasePlayer {
        inline uint64_t lifestate = 0x278;  // from dump: BaseCombatEntity::lifeState = 0x278
        inline uint64_t displayName = 0x348;  // from dump: username = 0x348
        inline uint64_t playerModel = 0x2C0;  // from dump: playerModel = 0x2c0
        inline uint64_t clActiveItem = 0x518;  // from dump: clActiveItem = 0x518
        inline uint64_t playerInput = 0x3B0;  // from dump: playerInput = 0x3b0
        inline uint64_t player_eyes = 0x2A8;
        inline uint64_t playerFlags = 0x660;  // from dump: playerFlags = 0x660
        inline uint64_t BaseMovement = 0x4D8;  // from dump: BaseMovement = 0x4D8
        inline uint64_t ModelState = 0x468;  // from dump: ModelState = 0x468
        inline uint64_t currentTeam = 0x4E8;  // from dump: team = 0x4e8
        inline uint64_t userId = 0x2D0;  // from dump: userId = 0x2D0
        inline uint64_t metabolism = 0x468;
        inline uint64_t blueprints = 0x328;
        inline uint64_t playerInventory = 0x628;  // from dump: playerInventory = 0x628
    }

    namespace ItemContainer {
        inline uint64_t list = 0x70;  // from dump
        inline uint64_t itemArray = 0x10;
        inline uint64_t itemCount = 0x18;
    }

    namespace PlayerEyes {
        inline uint64_t bodyRotation = 0x50;
        inline uint64_t viewOffset = 0x40;
        inline uint64_t eyeRotation = 0x0;
        inline uint64_t eye_offset = 0x28;
    }

    namespace PlayerInput {
        inline uint64_t bodyAngles = 0x44;
    }

    namespace Item {
        inline uint64_t item_definition = 0x68;  // from dump: itemDefinition = 0x68
        inline uint64_t itemid = 0x48;
        inline uint64_t held_entity = 0x30;  // from dump: heldEntity = 0x30
        inline uint64_t heldEntity2 = 0x48;  // from dump: heldEntity2 = 0x48
        inline uint64_t amount = 0x94;
        inline uint64_t health = 0;
        inline uint64_t maxHealth = 0;
        inline uint64_t parentContainer = 0x78;
        inline uint64_t itemUid = 0x10;  // from dump
        inline uint64_t itemUid2 = 0x20;  // from dump
        inline uint64_t itemUid3 = 0x28;  // from dump
        inline uint64_t itemUid4 = 0x8C;  // from dump
    }

    namespace Magazine {
        inline uint64_t definition = 0x10;
        inline uint64_t capacity = 0x18;
        inline uint64_t contents = 0x1C;
        inline uint64_t ammoType = 0x20;
    }

    namespace PlayerModel {
        inline uint64_t position = 0x210;  // from dump: position = 0x210
        inline uint64_t new_velocity = 0;
        inline uint64_t velocity = 0x234;  // from dump: velocity = 0x234
        inline uint64_t rotation = 0x228;
        inline uint64_t SkinnedMultiMesh = 0x348;  // from dump: SkinnedMultiMesh = 0x348
        inline uint64_t BoneTransforms = 0x60;   // fixed: was missing
        inline uint64_t HeadBone = 0x48;   // fixed: was missing
        inline uint64_t isVisible = 0x94;
        inline uint64_t IsNpc = 0x3DD;
    }

    namespace Model {
        inline uint64_t rootBone = 0x28;
        inline uint64_t headBone = 0x30;
        inline uint64_t eyeBone = 0x38;
        inline uint64_t boneTransforms = 0x50;
        inline uint64_t skeleton = 0x48;
    }

    namespace SkinnedMultiMesh {
        inline uint64_t render = 0x358;
        inline uint64_t rendererList = 0x40;
    }

    namespace BaseProjectile {
        inline uint64_t primaryMagazine = 0x388;  // from dump: primaryMagazine = 0x388
        inline uint64_t recoilProperties = 0x3B0;  // from dump: recoilProp = 0x3b0
        inline uint64_t automatic = 0x338;
        inline uint64_t projectileVelocityScale = 0x334;
        inline uint64_t repeatDelay = 0x294;
        inline uint64_t reloadDuration = 0x378;
        inline uint64_t damageScale = 0x32C;
        inline uint64_t aimCone = 0x3B8;
        inline uint64_t hipAimCone = 0x3BC;
        inline uint64_t aimconePenaltyPerShot = 0x3C0;
        inline uint64_t aimConePenaltyMax = 0x3C4;
        inline uint64_t numShotsFired = 0x3F4;
        inline uint64_t noHeadshots = 0x2E6;
    }

    namespace ItemDefinition {
        inline uint64_t shortname = 0x28;
        inline uint64_t itemid = 0x20;
        inline uint64_t ItemDisplayName = 0x40;
    }

    namespace Projectile {
        inline uint64_t drag = 0x34;
        inline uint64_t gravityModifier = 0x38;
        inline uint64_t initialVelocity = 0x28;
        inline uint64_t currentVelocity = 0x28;
        inline uint64_t thickness = 0x3C;
        inline uint64_t owner = 0x120;
    }

    namespace ItemModProjectile {
        inline uint64_t projectilevelocity = 0x40;
        inline uint64_t projectileSpread = 0x3C;
    }

    namespace RecoilProperties {
        inline uint64_t recoilYawMin = 0x18;
        inline uint64_t recoilYawMax = 0x1C;
        inline uint64_t recoilPitchMin = 0x20;
        inline uint64_t recoilPitchMax = 0x24;
    }

    namespace ModelState {
        inline uint64_t flags = 0x5C;
        inline uint64_t Ducked = 0x1;
        inline uint64_t Sprinting = 0x10;
        inline uint64_t Aiming = 0x80;
    }

    namespace WorldItem {
        inline uint64_t item = 0x1E0;
    }

    namespace ItemPickup {
        inline uint64_t itemdef = 0x350;
    }

    namespace BuildingBlock {
        inline uint64_t grade = 0x330;
    }

    namespace AmbientParams {
        inline uint64_t saturation = 0x10;
        inline uint64_t updateinterval = 0x1C;
    }

    // -----------------------------------------------------------------------
    // Legacy aliases - keep these so existing code compiles unchanged
    // -----------------------------------------------------------------------
    inline uint64_t base = Il2cpp::base;
    inline uint64_t entitylist = base_networkable::typeinfo;
    inline uint64_t staticfields = base_networkable::static_fields;
    inline uint64_t wrapperclass = main_camera::wrapper_class;
    inline uint64_t parentstaticfields = main_camera::parent_static_fields;
    inline uint64_t instance = main_camera::instance;
    inline uint64_t viewmatrix = main_camera::viewMatrix;
    inline uint64_t cameraposition = main_camera::position;
    inline uint64_t dayparams = TodSky::TOD_DayParameters;
    inline uint64_t nightparams = TodSky::TOD_NightParameters;
    inline uint64_t ambientparams = TodSky::TOD_AmbientParameters;
    inline uint64_t ambientcolor = TOD_LightParameters::ambientColor;
    inline uint64_t fogcolor = TOD_FogParameters::fogColor;
    inline uint64_t cloudcolor = TOD_LightParameters::cloudColor;
    inline uint64_t item = Item::held_entity;
    inline uint64_t itemdef = Item::item_definition;

    namespace EntityList {
        inline uint64_t base = base_networkable::typeinfo;
        inline uint64_t staticfields = base_networkable::static_fields;
        inline uint64_t wrapperclass = 0x0;
        inline uint64_t parentstaticfields = 0x10;
        inline uint64_t entitylist = base_networkable::client_entities;
    }

    namespace Camera {
        inline uint64_t base = main_camera::typeinfo;
        inline uint64_t staticfields = main_camera::static_fields;
        inline uint64_t wrapperclass = main_camera::wrapper_class;
        inline uint64_t parentstaticfields = main_camera::parent_static_fields;
        inline uint64_t viewmatrix = main_camera::viewMatrix;
        inline uint64_t cameraposition = main_camera::position;
    }

    namespace TODSky {
        inline uint64_t base = TodSky::Class;
        inline uint64_t staticfields = TodSky::static_fields;
        inline uint64_t instance = TodSky::instance;
        inline uint64_t wrapperclass = TodSky::wrapper_class;
        inline uint64_t parentstaticfields = TodSky::parent_static_fields;
        inline uint64_t dayparams = TodSky::TOD_DayParameters;
        inline uint64_t nightparams = TodSky::TOD_NightParameters;
        inline uint64_t ambientparams = TodSky::TOD_AmbientParameters;
    }

    namespace TODModify {
        inline uint64_t ambientcolor = TOD_LightParameters::ambientColor;
        inline uint64_t fogcolor = TOD_FogParameters::fogColor;
        inline uint64_t cloudcolor = TOD_LightParameters::cloudColor;
        inline uint64_t skycolor = TOD_LightParameters::skyColor;
        inline uint64_t raycolor = TOD_LightParameters::rayColor;
        inline uint64_t lightcolor = TOD_LightParameters::lightColor;
        inline uint64_t mooncolor = TOD_LightParameters::lightColor;
    }

    namespace NightParams {
        inline uint64_t runtimeambientmultiplier = TOD_NightParameters::ambientMultiplier;
    }

    // Legacy member spellings
    namespace BaseEntity {
        inline uint64_t isvisible = Visible;
        inline uint64_t isanimatorvisible = Visible;
        inline uint64_t isshadowvisible = Visible;
    }
    namespace BasePlayer {
        inline uint64_t displayname = displayName;
        inline uint64_t playermodel = playerModel;
        inline uint64_t playerinput = playerInput;
        inline uint64_t playerflags = playerFlags;
        inline uint64_t clactiveitem = clActiveItem;
    }
    namespace PlayerEyes { inline uint64_t bodyrotation = bodyRotation; }
    namespace PlayerInput { inline uint64_t bodyangles = bodyAngles; }
    namespace Item {
        inline uint64_t itemdefinition = item_definition;
        inline uint64_t heldentity = held_entity;
    }
    namespace Magazine { inline uint64_t ammotype = ammoType; }
    namespace BaseProjectile { inline uint64_t primarymagazine = primaryMagazine; }
    namespace PlayerModel {
        inline uint64_t newvelocity = new_velocity;
        inline uint64_t skinnedmultimesh = SkinnedMultiMesh;
    }
    namespace Model { inline uint64_t rootbone = rootBone; }
    namespace Projectile { inline uint64_t gravitymodifier = gravityModifier; }

} // namespace offsets