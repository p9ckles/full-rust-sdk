#pragma once

#include "include.h"

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

class c_physx
{
public:
	struct triangle_t
	{
		vector3 v0;
		vector3 v1;
		vector3 v2;

		auto center() const -> vector3
		{
			return (v0 + v1 + v2) / 3.00f;
		}
	};

	enum class px_concrete_type : std::uint16_t
	{
		e_undefined,

		e_heightfield,
		e_convex_mesh,
		e_triangle_mesh_bvh33,
		e_triangle_mesh_bvh34,
		e_cloth_fabric,

		e_rigid_dynamic,
		e_rigid_static,
		e_shape,
		e_material,
		e_constraint,
		e_cloth,
		e_particle_system,
		e_particle_fluid,
		e_aggregate,
		e_articulation,
		e_articulation_link,
		e_articulation_joint,
		e_pruning_structure,

		e_physx_core_count,
		e_first_physx_extension = 256,
		e_first_vehicle_extension = 512,
		e_first_user_extension = 1024
	};

	enum class px_geometry_type : std::int32_t
	{
		e_sphere,
		e_plane,
		e_capsule,
		e_box,
		e_convexmesh,
		e_trianglemesh,
		e_heightfield,

		e_geometry_count,
		e_invalid = -1
	};

	enum class px_base_flag : std::uint16_t
	{
		e_owns_memory = (1 << 0),
		e_is_releasable = (1 << 1)
	};

	enum class px_rigid_body_flag : std::uint8_t
	{
		e_kinematic = (1 << 0),
		e_use_kinematic_target_for_scene_queries = (1 << 1),
		e_enable_ccd = (1 << 2),
		e_enable_ccd_friction = (1 << 3),
		e_enable_pose_integration_preview = (1 << 4),
		e_enable_speculative_ccd = (1 << 5),
		e_enable_ccd_max_contact_impulse = (1 << 6)
	};

	enum class px_rigid_dynamic_lock_flags : std::uint16_t
	{
		e_lock_linear_x = (1 << 0),
		e_lock_linear_y = (1 << 1),
		e_lock_linear_z = (1 << 2),
		e_lock_angular_x = (1 << 3),
		e_lock_angular_y = (1 << 4),
		e_lock_angular_z = (1 << 5)
	};

	struct aabb_t
	{
		vector3 min;
		vector3 max;

		aabb_t()
			: min(FLT_MAX, FLT_MAX, FLT_MAX),
			max(-FLT_MAX, -FLT_MAX, -FLT_MAX)
		{
		}

		auto expand(const vector3& p) -> void
		{
			min.x = std::min(min.x, p.x);
			min.y = std::min(min.y, p.y);
			min.z = std::min(min.z, p.z);

			max.x = std::max(max.x, p.x);
			max.y = std::max(max.y, p.y);
			max.z = std::max(max.z, p.z);
		}

		auto intersects(const vector3& origin, const vector3& dir, float& tmin, float& tmax) const -> bool
		{
			tmin = 0.0f;
			tmax = FLT_MAX;

			for (int i = 0; i < 3; ++i)
			{
				float invD = 1.00f / (&dir.x)[i];
				float t0 = ((&min.x)[i] - (&origin.x)[i]) * invD;
				float t1 = ((&max.x)[i] - (&origin.x)[i]) * invD;
				if (invD < 0.00f)
					std::swap(t0, t1);

				tmin = t0 > tmin ? t0 : tmin;
				tmax = t1 < tmax ? t1 : tmax;
				if (tmax <= tmin)
					return false;
			}
			return true;
		}

		auto longest_axis() const -> int
		{
			vector3 extents = max - min;
			if (extents.x > extents.y && extents.x > extents.z)
				return 0;

			if (extents.y > extents.z)
				return 1;

			return 2;
		}
	};

	struct px_matrix_3x3_t
	{
		vector3 column0{};
		vector3 column1{};
		vector3 column2{};

		px_matrix_3x3_t() = default;

		px_matrix_3x3_t(const vector4& q)
		{
			const float x = q.x;
			const float y = q.y;
			const float z = q.z;
			const float w = q.w;

			const float x2 = x + x;
			const float y2 = y + y;
			const float z2 = z + z;

			const float xx = x2 * x;
			const float yy = y2 * y;
			const float zz = z2 * z;

			const float xy = x2 * y;
			const float xz = x2 * z;
			const float xw = x2 * w;

			const float yz = y2 * z;
			const float yw = y2 * w;
			const float zw = z2 * w;

			column0 = vector3(1.00f - yy - zz, xy + zw, xz - yw);
			column1 = vector3(xy - zw, 1.00f - xx - zz, yz + xw);
			column2 = vector3(xz + yw, yz - xw, 1.00f - xx - yy);
		}

		px_matrix_3x3_t(const vector3& col0, const vector3& col1, const vector3& col2)
			: column0(col0), column1(col1), column2(col2)
		{
		}

		auto get_transpose() const -> px_matrix_3x3_t
		{
			const vector3 v0(column0.x, column1.x, column2.x);
			const vector3 v1(column0.y, column1.y, column2.y);
			const vector3 v2(column0.z, column1.z, column2.z);

			return px_matrix_3x3_t(v0, v1, v2);
		}

		auto transform(const vector3& other) const -> vector3
		{
			return column0 * other.x + column1 * other.y + column2 * other.z;
		}

		auto operator*(const px_matrix_3x3_t& other) const -> px_matrix_3x3_t
		{
			return px_matrix_3x3_t(transform(other.column0), transform(other.column1), transform(other.column2));
		}

		auto operator*(const vector3& vec) const -> vector3
		{
			return transform(vec);
		}
	};

	struct px_matrix_3x4_t
	{
		px_matrix_3x3_t m{};
		vector3 p{};

		px_matrix_3x4_t(const px_matrix_3x3_t& other, const vector3& t)
			: m(other), p(t)
		{
		}

		vector3 transform(const vector3& other) const
		{
			return m * other + p;
		}
	};

	struct px_mesh_scale_t
	{
		px_mesh_scale_t() : scale(1.00f, 1.00f, 1.00f), rotation()
		{
		}

		px_mesh_scale_t(float r) : scale(r, r, r), rotation()
		{
		}

		px_mesh_scale_t(const vector3& s)
		{
			scale = s;
			rotation = vector4();
		}

		px_mesh_scale_t(const vector3& s, const vector4& r)
		{
			scale = s;
			rotation = r;
		}

		auto get_inverse() const -> px_mesh_scale_t
		{
			return px_mesh_scale_t(vector3(1.00f / scale.x, 1.00f / scale.y, 1.00f / scale.z), rotation);
		}

		auto to_mat33() const -> px_matrix_3x3_t
		{
			px_matrix_3x3_t rot(rotation);
			px_matrix_3x3_t trans = rot.get_transpose();
			trans.column0 *= scale.x;
			trans.column1 *= scale.y;
			trans.column2 *= scale.z;
			return trans * rot;
		}

		vector3		scale;
		vector4		rotation;
	};

	struct px_transform_t
	{
		vector4 m_rotation{};
		vector3 m_position{};

		px_transform_t() = default;

		px_transform_t(const vector4& rotation, const vector3& position)
			: m_rotation(rotation), m_position(position)
		{
		}

		auto transform(const px_transform_t& src) const -> px_transform_t
		{
			px_transform_t ret_transform{};
			ret_transform.m_rotation = m_rotation * src.m_rotation;
			ret_transform.m_position = m_rotation.rotate(src.m_position) + m_position;
			return ret_transform;
		}

		auto operator* (const px_transform_t& src) const -> px_transform_t
		{
			return transform(src);
		}

		auto get_inverse() const -> px_transform_t
		{
			px_transform_t ret_transform{};
			ret_transform.m_rotation = m_rotation.conjugate();
			ret_transform.m_position = m_rotation.rotate_inv({ -m_position.x, -m_position.y, -m_position.z });
			return ret_transform;
		}

		auto transform_point(const vector3& point) const -> vector3
		{
			return m_rotation.rotate(point) + m_position;
		}

		auto inverse_transform_point(const vector3& point) const -> vector3
		{
			return m_rotation.rotate_inv(point - m_position);
		}

		auto transform_direction(const vector3& direction) const -> vector3
		{
			return m_rotation.rotate(direction);
		}

		auto inverse_transform_direction(const vector3& direction) const -> vector3
		{
			return m_rotation.rotate_inv(direction);
		}

		auto inverse_transform(const vector3& point) const -> vector3
		{
			return m_rotation.rotate_inv(point - m_position);
		}

		auto rotate(const vector3& direction) const -> vector3
		{
			return m_rotation.rotate(direction);
		}
	};

	template <typename storage_type, storage_type bit_mask>
	class px_bit_and_data_t
	{
	public:
		__forceinline px_bit_and_data_t() : m_data(0)
		{
		}
	protected:
		storage_type m_data;
	};
	typedef px_bit_and_data_t<unsigned char, 0x80> px_bit_and_byte;
	typedef px_bit_and_data_t<unsigned short, 0x8000> px_bit_and_word;
	typedef px_bit_and_data_t<unsigned int, 0x80000000> px_bit_and_dword;

	struct ptr_table_t
	{
		union
		{
			void* m_single;
			void** m_list;
		};

		std::uint16_t m_count;
		bool		  m_owns_memory;
		bool		  m_buffer_used;

		auto get_count() -> std::uint32_t
		{
			return m_count;
		}

		auto get_ptrs() -> void**
		{
			return m_count == 1 ? &m_single : m_list;
		}
	};

	template <typename t>
	struct array_t
	{
		t** m_data;
		std::uint32_t m_capacity;
		std::uint32_t m_size;
	};

	struct filter_data_t
	{
		std::uint32_t word0;
		std::uint32_t word1;
		std::uint32_t word2;
		std::uint32_t word3;
	};

	struct px_actor_t
	{
		std::byte			_pad[0x8]{};
		px_concrete_type	m_type{};
		px_base_flag		m_base_flags{};
		std::uint64_t		m_user_data{};
	};

	struct pruning_structure_t
	{
		std::byte		  _pad[0x8]{};
		px_concrete_type  m_type{};
		std::uint16_t     m_base_flags{};
		std::uint32_t     m_nb_nodes[2]{};
		void* m_aabb_tree_nodes[2]{};
		std::uint32_t     m_nb_objects[2]{};
		std::uint32_t* m_aabb_tree_indices[2]{};
		std::uint32_t     m_nb_actors{};
		px_actor_t** m_actors{};
		bool              m_valid{};
	};

	struct np_shape_manager_t
	{
		ptr_table_t             m_shapes{};
		ptr_table_t             m_scene_query_data{};
		pruning_structure_t* m_pruning_structure{};
	};

	struct px_rigid_actor_t : px_actor_t
	{
		const char* m_name{};
		void* m_connector_array{};
		np_shape_manager_t    m_shape_manager{};
		std::uint32_t         m_index{};
	};

	struct px_geometry_t
	{
		px_geometry_type m_type{};
	};

	struct geometry_union_t
	{
		union
		{
			void* m_alignment{};
			std::uint8_t    m_box[16];
			std::uint8_t    m_sphere[8];
			std::uint8_t    m_capsule[12];
			std::uint8_t    m_plane[4];
			std::uint8_t    m_convex[64];
			std::uint8_t    m_mesh[80];
			std::uint8_t    m_heightfield[56];
			std::uint8_t    m_invalid[4];
		} m_geometry;

		auto get_type() const -> px_geometry_type
		{
			const auto& base = reinterpret_cast<const px_geometry_t&>(m_geometry);
			if (base.m_type < px_geometry_type::e_sphere || base.m_type > px_geometry_type::e_heightfield)
				return px_geometry_type::e_invalid;

			return base.m_type;
		}
	};

	struct px_shape_core_t
	{
		alignas(16)		px_transform_t  m_transform {};
		float               m_contact_offset{};
		std::uint8_t        m_shape_flags{};
		std::uint8_t        m_owns_material_idx_memory{};
		std::uint16_t       m_material_index{};
		geometry_union_t    m_geometry{};
	};

	struct shape_core_t
	{
		filter_data_t	m_query_filter_data{};
		filter_data_t	m_simulation_filter_data{};
		alignas(16)	px_shape_core_t m_core {};
		float			m_rest_offset{};
	};

	struct shape_t
	{
		std::uintptr_t    m_scene{};
		std::uint32_t     m_control_state{};
		std::uintptr_t    m_stream_ptr{};
		shape_core_t      m_shape_core{};
	};

	struct np_shape_t : px_actor_t
	{
		std::uintptr_t      m_ref_countable{};
		std::int32_t        m_ref_count{};
		px_rigid_actor_t* m_actor{};
		shape_t             m_shape{};
		const char* m_name{};
		std::int32_t        m_exclusive_and_actor_count{};

		auto get_geometry_type() const -> px_geometry_type
		{
			return m_shape.m_shape_core.m_core.m_geometry.get_type();
		}
	};

	struct px_rigid_core_t
	{
		alignas(16)		px_transform_t m_body_to_world {};
		px_rigid_body_flag	m_flags{};
		std::uint8_t		m_idt_body_to_actor{};
		std::uint16_t		m_solver_iteration_counts{};
	};

	struct rigid_core_t
	{
		std::byte _pad[0x10]{};
	};

	struct pxs_body_core_t : px_rigid_core_t
	{
		alignas(16)					px_transform_t m_body_to_actor;
		float							m_ccd_advance_coefficient;

		vector3						m_linear_velocity;
		float							m_max_pen_bias;

		vector3						m_angular_velocity;
		float							m_contact_report_threshold;

		float							m_max_angular_velocity_sq;
		float							m_max_linear_velocity_sq;
		float							m_linear_damping;
		float							m_angular_damping;

		vector3						m_inverse_inertia;
		float							m_inverse_mass;

		float							m_max_contact_impulse;
		float							m_sleep_threshold;
		float							m_freeze_threshold;
		float							m_wake_counter;

		float							m_solver_wake_counter;
		std::uint32_t					m_num_counted_interactions;
		std::uint32_t					m_num_body_interactions;
		std::uint16_t					m_is_fast_moving;
		px_rigid_dynamic_lock_flags     m_lock_flags;
	};

	struct body_core_t : rigid_core_t
	{
		alignas(16)		pxs_body_core_t m_core {};
		void* m_sim_state_data{};
	};

	struct body_t
	{
		std::uint64_t       m_scene{};
		std::uint64_t       m_control_state{};
		std::uint64_t       m_stream_ptr{};
		body_core_t         m_body_core{};
		alignas(16)		px_transform_t m_buffered_body2_world;
		vector3         m_buffered_lin_velocity;
		vector3         m_buffered_ang_velocity;
		float               m_buffered_wake_counter{};
		std::uint32_t       m_buffered_is_sleeping{};
		std::uint32_t       m_body_buffer_flags{};
	};

	struct np_rigid_static_t : px_rigid_actor_t
	{
		body_t m_rigid_static{};

		auto get_global_pose() const -> px_transform_t
		{
			const auto state = (m_rigid_static.m_control_state & 40);
			if (state != 0)
				return m.read<pxs_body_core_t>(m.read<std::uintptr_t>(m_rigid_static.m_stream_ptr + 0xB0)).m_body_to_world;
			else
				return m_rigid_static.m_body_core.m_core.m_body_to_world;
		}
	};

	struct static_core_t
	{
		std::byte		 _pad[0x10]{};
		px_rigid_core_t  m_core{};
	};

	struct rigid_static_t
	{
		std::byte	  _pad[0x16]{};
		static_core_t m_static{};
	};

	struct np_scene_t
	{
		std::byte				   _pad[0x23C8];
		array_t<px_rigid_actor_t*> m_rigid_actors;
	};

	struct np_physics_t
	{
		std::byte			 _pad[0x8]{};
		array_t<np_scene_t*> m_scene_array{};
	};

	struct bvhnode_t
	{
		aabb_t						bounds;
		std::vector<std::size_t>	triangle_indices;
		std::unique_ptr<bvhnode_t>  left, right;
	};

	struct actor_information_t
	{
		std::vector<triangle_t>		m_triangles{};
		px_transform_t				m_transform{};
		px_geometry_type			m_type{};
		aabb_t						m_bounds;
		std::unique_ptr<bvhnode_t>  m_bvh_root;
	};

	struct px_height_field_sample_t
	{
		std::int16_t	m_height{};
		px_bit_and_byte m_material_index_0{};
		px_bit_and_byte m_material_index_1{};
	};

	struct center_extents_t
	{
		vector3 m_center{};
		vector3 m_extents{};
	};

	struct height_field_data_t
	{
		center_extents_t			m_aabb{};
		std::uint32_t				m_rows{};
		std::uint32_t				m_columns{};
		float						m_row_limit{};
		float						m_column_limit{};
		float						m_nb_columns{};
		px_height_field_sample_t* m_samples{};
		float						m_thickness{};
		float						m_convex_edge_threshold{};
		std::uint16_t				m_flags{};
		std::uint8_t				m_format{};
	};

	struct triangle_mesh_t
	{
		std::byte           _pad[0x8]{};
		px_concrete_type    m_type{};
		px_base_flag        m_base_flags{};
		std::uintptr_t      m_ref_countable_vfptr{};
		std::int64_t        m_ref_count{};

		std::uint32_t       m_nb_vertices{};
		std::uint32_t       m_nb_triangles{};

		vector3* m_vertices{};
		void* m_triangles{};
		center_extents_t    m_aabb{};

		std::uint8_t* m_extra_trig_data{};
		float               m_geom_epsilon{};
		std::uint8_t        m_flags{};
		std::uint16_t* m_material_indices{};

		std::uint32_t* m_face_remap{};
		std::uint32_t* m_adjacencies{};
		std::uint64_t* m_mesh_factory{};

		void* m_grb_triangle_indices{};
		void* m_grb_triangle_adjacencies{};
		std::uint32_t* m_grb_face_remap{};
		void* m_grb_bv32_tree{};
	};

	struct px_triangle_mesh_geometry_t : px_geometry_t
	{
		px_mesh_scale_t     m_scale{};
		std::uint8_t        m_mesh_flags{};
		std::byte           _pad[0x3]{};
		triangle_mesh_t* m_triangle_mesh{};
	};

	struct px_box_geometry_t : px_geometry_t
	{
		vector3 m_half_extents{};
	};

	struct px_capsule_geometry_t : px_geometry_t
	{
		float m_radius{};
		float m_half_height{};
	};

	struct px_sphere_geometry_t : px_geometry_t
	{
		float m_radius{};
	};

	struct physx_hit_t
	{
		actor_information_t		m_collider{};
		vector3				m_point{};
		vector3				m_normal{};
		float					m_distance{};
		bool					m_did_hit{};
	};

	inline auto generate_box_triangles
	(
		const px_transform_t& transform, const vector3& half_extents
	) -> std::vector<triangle_t>
	{
		std::array<vector3, 8> corners;
		int idx = 0;
		for (int x : { -1, 1 })
		{
			for (int y : { -1, 1 })
			{
				for (int z : { -1, 1 })
				{
					corners[idx++] = transform.transform_point({
						x * half_extents.x, y * half_extents.y, z * half_extents.z });
				}
			}
		}

		constexpr std::array<std::array<int, 3>, 12> faces =
		{ {
			{0, 1, 2}, {1, 3, 2}, // -Z
			{4, 6, 5}, {5, 6, 7}, // +Z
			{0, 2, 4}, {2, 6, 4}, // -X
			{1, 5, 3}, {3, 5, 7}, // +X
			{0, 4, 1}, {1, 4, 5}, // -Y
			{2, 3, 6}, {3, 7, 6}  // +Y
		} };

		std::vector<triangle_t> tris;
		tris.reserve(12);
		for (const auto& f : faces)
		{
			const auto& v0 = corners[f[0]];
			const auto& v1 = corners[f[1]];
			const auto& v2 = corners[f[2]];

			const auto normal = (v1 - v0).cross(v2 - v0);
			if (normal.length_squared() < 1e-6f)
				continue;

			tris.push_back({ v0, v1, v2 });
		}

		return tris;
	}

	inline auto generate_triangle_mesh_triangles
	(
		const c_physx::px_transform_t& transform,
		const c_physx::px_triangle_mesh_geometry_t& geometry
	) -> std::vector<triangle_t>
	{
		std::vector<triangle_t> triangles;

		const auto* mesh_ptr = geometry.m_triangle_mesh;
		if (!mesh_ptr)
			return triangles;

		const auto mesh = m.read<c_physx::triangle_mesh_t>(
			reinterpret_cast<std::uintptr_t>(mesh_ptr)
		);

		if (mesh.m_nb_vertices == 0 || mesh.m_nb_triangles == 0 ||
			!mesh.m_vertices || !mesh.m_triangles)
			return triangles;

		const auto vertices = m.read_vec<vector3>(
			reinterpret_cast<std::uintptr_t>(mesh.m_vertices),
			mesh.m_nb_vertices
		);
		if (vertices.size() != mesh.m_nb_vertices)
			return triangles;

		std::vector<std::uint32_t> indices;
		const bool has16_bit_indices = mesh.m_flags & 2u;

		if (has16_bit_indices)
		{
			const auto small_indices = m.read_vec<std::uint16_t>(
				reinterpret_cast<std::uintptr_t>(mesh.m_triangles),
				mesh.m_nb_triangles * 3
			);
			if (small_indices.size() != mesh.m_nb_triangles * 3)
				return triangles;

			indices.reserve(small_indices.size());
			for (auto idx : small_indices)
				indices.push_back(static_cast<std::uint32_t>(idx));
		}
		else
		{
			indices = m.read_vec<std::uint32_t>(
				reinterpret_cast<std::uintptr_t>(mesh.m_triangles),
				mesh.m_nb_triangles * 3
			);
			if (indices.size() != mesh.m_nb_triangles * 3)
				return triangles;
		}

		const auto scale_matrix = geometry.m_scale.to_mat33();

		triangles.reserve(mesh.m_nb_triangles);
		for (std::size_t idx = 0; idx + 2 < indices.size(); idx += 3)
		{
			const auto i0 = indices[idx + 0];
			const auto i1 = indices[idx + 1];
			const auto i2 = indices[idx + 2];

			if (i0 >= vertices.size() || i1 >= vertices.size() || i2 >= vertices.size())
				continue;

			const auto& v0 = vertices[i0];
			const auto& v1 = vertices[i1];
			const auto& v2 = vertices[i2];

			if (v0.is_empty() || v1.is_empty() || v2.is_empty())
				continue;

			const auto s0 = scale_matrix.transform(v0);
			const auto s1 = scale_matrix.transform(v1);*-
			const auto s2 = scale_matrix.transform(v2);

			if (s0.is_empty() || s1.is_empty() || s2.is_empty())
				continue;

			const auto w0 = transform.transform_point(s0);
			const auto w1 = transform.transform_point(s1);
			const auto w2 = transform.transform_point(s2);

			if (w0.is_empty() || w1.is_empty() || w2.is_empty())
				continue;

			const auto normal = (w1 - w0).cross(w2 - w0);
			if (normal.length_squared() < 1e-6f)
				continue;

			triangles.emplace_back(triangle_t{ w0, w1, w2 });
		}

		return triangles;
	}

	auto compute_aabb_indices(const std::vector<triangle_t>& triangles, const std::vector<size_t>& indices) -> aabb_t
	{
		aabb_t bounds;
		for (size_t idx : indices)
		{
			const auto& tri = triangles[idx];
			bounds.expand(tri.v0);
			bounds.expand(tri.v1);
			bounds.expand(tri.v2);
		}
		return bounds;
	}

	auto compute_aabb(const std::vector<triangle_t>& triangles) -> aabb_t
	{
		aabb_t bounds;
		for (const auto& tri : triangles)
		{
			bounds.expand(tri.v0);
			bounds.expand(tri.v1);
			bounds.expand(tri.v2);
		}
		return bounds;
	}

	auto build_bvh_from_indices
	(
		const std::vector<triangle_t>& triangles,
		std::vector<size_t>& indices,
		int depth = 0
	) -> bvhnode_t*
	{
		auto* node = new bvhnode_t();
		node->bounds = compute_aabb_indices(triangles, indices);

		if (indices.size() <= 16 || depth > 12)
		{
			node->triangle_indices = indices;
			return node;
		}

		int axis = node->bounds.longest_axis();
		auto mid = indices.begin() + indices.size() / 2;

		std::nth_element(indices.begin(), mid, indices.end(),
			[&](size_t a, size_t b)
			{
				return triangles[a].center()[axis] < triangles[b].center()[axis];
			});

		std::vector<std::size_t> left_indices(indices.begin(), mid);
		std::vector<std::size_t> right_indices(mid, indices.end());

#pragma omp parallel sections
		{
#pragma omp section
			node->left.reset(build_bvh_from_indices(triangles, left_indices, depth + 1));

#pragma omp section
			node->right.reset(build_bvh_from_indices(triangles, right_indices, depth + 1));
		}

		return node;
	}

	auto build_actor_bvh(actor_information_t& actor) -> void
	{
		if (actor.m_triangles.empty()) return;

		std::vector<std::size_t> indices(actor.m_triangles.size());
		std::iota(indices.begin(), indices.end(), 0); // Fill [0, 1, 2, ...]

		actor.m_bounds = compute_aabb(actor.m_triangles);
		actor.m_bvh_root.reset(build_bvh_from_indices(actor.m_triangles, indices));
	}

	std::atomic<std::shared_ptr<std::vector<actor_information_t>>> m_actors_atomic;

	auto cache_actors() -> void
	{
		auto new_actors = std::make_shared<std::vector<actor_information_t>>();

		const auto sdk_ptr = m.read<std::uint64_t>(globals::unity_player + 0x1C3B3D0);
		if (!sdk_ptr)
			return;

		const auto physx_sdk = m.read<np_physics_t>(sdk_ptr);
		if (!physx_sdk.m_scene_array.m_size || !physx_sdk.m_scene_array.m_data)
			return;

		const auto physx_scene = m.read_vec<std::uint64_t>(
			reinterpret_cast<std::uintptr_t>(physx_sdk.m_scene_array.m_data),
			physx_sdk.m_scene_array.m_size
		);
		if (physx_scene.empty())
			return;

		for (const auto& scene_ptr : physx_scene)
		{
			const auto rigid_actors = m.read<decltype(np_scene_t::m_rigid_actors)>(
				scene_ptr + offsetof(np_scene_t, m_rigid_actors)
			);
			if (!rigid_actors.m_data || rigid_actors.m_size == 0)
				continue;

			const auto actor_ptrs = m.read_vec<std::uint64_t>(
				reinterpret_cast<std::uintptr_t>(rigid_actors.m_data),
				rigid_actors.m_size
			);
			if (actor_ptrs.empty())
				continue;

			for (const auto& actor_ptr : actor_ptrs)
			{
				const auto actor = m.read<px_actor_t>(actor_ptr);
				if (actor.m_type != px_concrete_type::e_rigid_dynamic)
					continue;

				const auto rigid_static = m.read<np_rigid_static_t>(actor_ptr);
				if (!rigid_static.m_shape_manager.m_shapes.m_single)
					continue;

				const auto shape = m.read<np_shape_t>(
					reinterpret_cast<std::uint64_t>(rigid_static.m_shape_manager.m_shapes.m_single)
				);
				if (!shape.m_shape.m_scene)
					continue;

				actor_information_t new_inf{};
				new_inf.m_type = shape.get_geometry_type();
				new_inf.m_transform = rigid_static.get_global_pose();

				switch (new_inf.m_type)
				{
				case px_geometry_type::e_trianglemesh:
				{
					const auto& g = reinterpret_cast<const px_triangle_mesh_geometry_t&>(
						shape.m_shape.m_shape_core.m_core.m_geometry.m_geometry
						);
					new_inf.m_triangles = generate_triangle_mesh_triangles(new_inf.m_transform, g);
					break;
				}
				case px_geometry_type::e_box:
				{
					const auto& g = reinterpret_cast<const px_box_geometry_t&>(
						shape.m_shape.m_shape_core.m_core.m_geometry.m_geometry
						);
					new_inf.m_triangles = generate_box_triangles(new_inf.m_transform, g.m_half_extents);
					break;
				}
				default:
					break;
				}

				build_actor_bvh(new_inf);
				new_actors->push_back(std::move(new_inf));
			}
		}

		m_actors_atomic.store(new_actors, std::memory_order_release);
	}

	auto ray_intersects_triangle
	(
		const vector3& origin,
		const vector3& dir,
		const triangle_t& tri,
		float& out_t,
		vector3& out_normal
	) -> bool
	{
		const float EPSILON = 1e-6f;
		vector3 v0v1 = tri.v1 - tri.v0;
		vector3 v0v2 = tri.v2 - tri.v0;
		vector3 pvec = dir.cross(v0v2);
		float det = v0v1.dot(pvec);

		if (fabs(det) < EPSILON)
			return false;
		float invDet = 1.00f / det;

		vector3 tvec = origin - tri.v0;
		float u = tvec.dot(pvec) * invDet;
		if (u < 0.00f || u > 1.00f)
			return false;

		vector3 qvec = tvec.cross(v0v1);
		float v = dir.dot(qvec) * invDet;
		if (v < 0.00f || u + v > 1.00f)
			return false;

		float t = v0v2.dot(qvec) * invDet;
		if (t <= 0.00f)
			return false;

		out_t = t;
		out_normal = v0v1.cross(v0v2).normalize();
		return true;
	}

	auto raycast_bvh
	(
		const bvhnode_t* node,
		const vector3& origin,
		const vector3& dir,
		float& closest_t,
		physx_hit_t& best_hit,
		const std::vector<triangle_t>& triangles
	) -> bool
	{
		float tmin, tmax;
		if (!node->bounds.intersects(origin, dir, tmin, tmax))
			return false;

		bool hit_any = false;

		if (!node->left && !node->right)
		{
			for (size_t idx : node->triangle_indices)
			{
				const auto& tri = triangles[idx];
				float t;
				vector3 normal;
				if (ray_intersects_triangle(origin, dir, tri, t, normal) && t < closest_t)
				{
					closest_t = t;
					best_hit.m_point = origin + dir * t;
					best_hit.m_normal = normal;
					best_hit.m_distance = t;
					best_hit.m_did_hit = true;
					hit_any = true;
				}
			}
			return hit_any;
		}

		if (node->left)
			hit_any |= raycast_bvh(node->left.get(), origin, dir, closest_t, best_hit, triangles);

		if (node->right)
			hit_any |= raycast_bvh(node->right.get(), origin, dir, closest_t, best_hit, triangles);

		return hit_any;
	}

	auto raycast(vector3 origin, vector3 direction, float distance) -> physx_hit_t
	{
		physx_hit_t final_hit{};
		if (origin.is_empty() || direction.is_empty() || !distance)
			return final_hit;

		auto current_actors = m_actors_atomic.load(std::memory_order_acquire);
		if (!current_actors)
			return final_hit;

		float closest_t = distance;
		for (const auto& actor : *current_actors)
		{
			if (!actor.m_bvh_root)
				continue;

			float tmin, tmax;
			if (!actor.m_bounds.intersects(origin, direction, tmin, tmax))
				continue;

			raycast_bvh(actor.m_bvh_root.get(), origin, direction, closest_t, final_hit, actor.m_triangles);
		}

		return final_hit;
	}

	auto linecast(vector3 from, vector3 to) -> physx_hit_t
	{
		if (from.is_empty() || to.is_empty())
			return {};

		const auto max_dist = (to - from).length();
		return raycast(from, (to - from).normalize(), max_dist);
	}
};

const auto physx = new c_physx();
