######################################################################
# Automatically generated by qmake (2.01a) Sun Oct 30 00:14:35 2011
######################################################################

INCLUDEPATH += include
CONFIG += staticlib
OBJECTS_DIR = obj
DESTDIR = lib
TEMPLATE = lib
TARGET = 
DEPENDPATH += . \
              include/common \
              include/GL \
              include/glm \
              include/ik \
              include/interpolation \
              include/rigidbody \
              src/common \
              src/ik \
              src/interpolation \
              src/rigidbody \
              include/glm/core \
              include/glm/gtc \
              include/glm/gtx \
              include/glm/virtrev
INCLUDEPATH += . \
               include/common \
               include/rigidbody \
               include/ik \
               include/interpolation

# Input
HEADERS += include/common/Animator.h \
           include/common/Application.h \
           include/common/Camera.h \
           include/common/GameObject.h \
           include/common/GlutApplication.h \
           include/common/Keymap.h \
           include/common/Material.h \
           include/common/Mesh.h \
           include/common/MeshObject.h \
           include/common/MeshRenderer.h \
           include/common/Renderer.h \
           include/common/SDLApplication.h \
           include/common/Shader.h \
           include/common/Texture.h \
           include/common/Trace.h \
           include/common/Transform.h \
           include/common/Utils.h \
           include/GL/freeglut.h \
           include/GL/freeglut_ext.h \
           include/GL/freeglut_std.h \
           include/GL/glew.h \
           include/GL/glut.h \
           include/GL/glxew.h \
           include/GL/wglew.h \
           include/glm/ext.hpp \
           include/glm/glm.hpp \
           include/ik/Bone.h \
           include/ik/Skeleton.h \
           include/interpolation/ControlPoints.h \
           include/interpolation/Interpolator.h \
           include/interpolation/KeyframeAnimator.h \
           include/interpolation/SplineRenderer.h \
           include/interpolation/TransformRenderer.h \
           include/rigidbody/RigidBody.h \
           include/glm/core/_detail.hpp \
           include/glm/core/_fixes.hpp \
           include/glm/core/_swizzle.hpp \
           include/glm/core/func_common.hpp \
           include/glm/core/func_exponential.hpp \
           include/glm/core/func_geometric.hpp \
           include/glm/core/func_integer.hpp \
           include/glm/core/func_matrix.hpp \
           include/glm/core/func_noise.hpp \
           include/glm/core/func_packing.hpp \
           include/glm/core/func_trigonometric.hpp \
           include/glm/core/func_vector_relational.hpp \
           include/glm/core/hint.hpp \
           include/glm/core/intrinsic_common.hpp \
           include/glm/core/intrinsic_exponential.hpp \
           include/glm/core/intrinsic_geometric.hpp \
           include/glm/core/intrinsic_matrix.hpp \
           include/glm/core/intrinsic_trigonometric.hpp \
           include/glm/core/intrinsic_vector_relational.hpp \
           include/glm/core/setup.hpp \
           include/glm/core/type.hpp \
           include/glm/core/type_float.hpp \
           include/glm/core/type_gentype.hpp \
           include/glm/core/type_half.hpp \
           include/glm/core/type_int.hpp \
           include/glm/core/type_mat.hpp \
           include/glm/core/type_mat2x2.hpp \
           include/glm/core/type_mat2x3.hpp \
           include/glm/core/type_mat2x4.hpp \
           include/glm/core/type_mat3x2.hpp \
           include/glm/core/type_mat3x3.hpp \
           include/glm/core/type_mat3x4.hpp \
           include/glm/core/type_mat4x2.hpp \
           include/glm/core/type_mat4x3.hpp \
           include/glm/core/type_mat4x4.hpp \
           include/glm/core/type_size.hpp \
           include/glm/core/type_vec.hpp \
           include/glm/core/type_vec1.hpp \
           include/glm/core/type_vec2.hpp \
           include/glm/core/type_vec3.hpp \
           include/glm/core/type_vec4.hpp \
           include/glm/gtc/half_float.hpp \
           include/glm/gtc/matrix_access.hpp \
           include/glm/gtc/matrix_integer.hpp \
           include/glm/gtc/matrix_inverse.hpp \
           include/glm/gtc/matrix_transform.hpp \
           include/glm/gtc/quaternion.hpp \
           include/glm/gtc/swizzle.hpp \
           include/glm/gtc/type_precision.hpp \
           include/glm/gtc/type_ptr.hpp \
           include/glm/gtx/associated_min_max.hpp \
           include/glm/gtx/bit.hpp \
           include/glm/gtx/closest_point.hpp \
           include/glm/gtx/color_cast.hpp \
           include/glm/gtx/color_space.hpp \
           include/glm/gtx/color_space_YCoCg.hpp \
           include/glm/gtx/compatibility.hpp \
           include/glm/gtx/component_wise.hpp \
           include/glm/gtx/epsilon.hpp \
           include/glm/gtx/euler_angles.hpp \
           include/glm/gtx/extend.hpp \
           include/glm/gtx/extented_min_max.hpp \
           include/glm/gtx/fast_exponential.hpp \
           include/glm/gtx/fast_square_root.hpp \
           include/glm/gtx/fast_trigonometry.hpp \
           include/glm/gtx/gradient_paint.hpp \
           include/glm/gtx/handed_coordinate_space.hpp \
           include/glm/gtx/inertia.hpp \
           include/glm/gtx/int_10_10_10_2.hpp \
           include/glm/gtx/integer.hpp \
           include/glm/gtx/intersect.hpp \
           include/glm/gtx/log_base.hpp \
           include/glm/gtx/matrix_cross_product.hpp \
           include/glm/gtx/matrix_interpolation.hpp \
           include/glm/gtx/matrix_major_storage.hpp \
           include/glm/gtx/matrix_operation.hpp \
           include/glm/gtx/matrix_query.hpp \
           include/glm/gtx/mixed_product.hpp \
           include/glm/gtx/multiple.hpp \
           include/glm/gtx/noise.hpp \
           include/glm/gtx/norm.hpp \
           include/glm/gtx/normal.hpp \
           include/glm/gtx/normalize_dot.hpp \
           include/glm/gtx/number_precision.hpp \
           include/glm/gtx/ocl_type.hpp \
           include/glm/gtx/optimum_pow.hpp \
           include/glm/gtx/orthonormalize.hpp \
           include/glm/gtx/perpendicular.hpp \
           include/glm/gtx/polar_coordinates.hpp \
           include/glm/gtx/projection.hpp \
           include/glm/gtx/quaternion.hpp \
           include/glm/gtx/random.hpp \
           include/glm/gtx/raw_data.hpp \
           include/glm/gtx/reciprocal.hpp \
           include/glm/gtx/rotate_vector.hpp \
           include/glm/gtx/simd_mat4.hpp \
           include/glm/gtx/simd_vec4.hpp \
           include/glm/gtx/simplex.hpp \
           include/glm/gtx/spline.hpp \
           include/glm/gtx/std_based_type.hpp \
           include/glm/gtx/string_cast.hpp \
           include/glm/gtx/transform.hpp \
           include/glm/gtx/transform2.hpp \
           include/glm/gtx/ulp.hpp \
           include/glm/gtx/unsigned_int.hpp \
           include/glm/gtx/vec1.hpp \
           include/glm/gtx/vector_access.hpp \
           include/glm/gtx/vector_angle.hpp \
           include/glm/gtx/vector_query.hpp \
           include/glm/gtx/verbose_operator.hpp \
           include/glm/gtx/wrap.hpp \
           include/glm/virtrev/xstream.hpp
SOURCES += src/common/Camera.cpp \
           src/common/GameObject.cpp \
           src/common/GlutApplication.cpp \
           src/common/Material.cpp \
           src/common/Mesh.cpp \
           src/common/MeshFactory.cpp \
           src/common/MeshObject.cpp \
           src/common/MeshRenderer.cpp \
           src/common/SDLApplication.cpp \
           src/common/Shader.cpp \
           src/common/Texture.cpp \
           src/common/Transform.cpp \
           src/ik/Bone.cpp \
           src/ik/Skeleton.cpp \
           src/interpolation/KeyframeAnimator.cpp \
           src/interpolation/SplineRenderer.cpp \
           src/interpolation/TransformRenderer.cpp \
           src/rigidbody/RigidBody.cpp
