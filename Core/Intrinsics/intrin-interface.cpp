#include intrinsics.cpp
namespace intrin{
	namespace detail{
		#define PACK_TYPE(in_type,vector_type,recursion)\
			std::conditional_t<\
				std::is_same<T, in_type>{},\
				vector_type,\
				recursion\
			>

		template<typename T>
		class ERROR_MSG_PACK_TYPE{
			static_assert(sizeof(T)<0, "pack does not support your underlying type-yet");
		};
		template<int size>
		class ERROR_MSG_PACK_SIZE{
			static_assert(size==128||size==256, "Pack has to be of size 128 or 256");
		};
	}

template<typename T, int n>
using pack = std::conditional_t<
    sizeof(T)*n==16,
    PACK_TYPE( int16_t, vec_int16x8 ,
    PACK_TYPE(uint16_t, vec_uint16x8,
    PACK_TYPE( int32_t, vec_int32x8 ,
    PACK_TYPE(uint32_t, vec_uint32x8,
    PACK_TYPE(float   , vec_floatx4 ,
    PACK_TYPE(double  , vec_doublex2,
    detail::ERROR_MSG_PACK_TYPE<T>
    )))))),
    std::conditional_t<
    sizeof(T)*n==32,
    detail::ERROR_MSG_PACK_TYPE<T>, //No 256bit vectors are implemented yet
    detail::ERROR_MSG_PACK_SIZE<sizeof(T)*n*8> //Only 128bit and 256bit vectors will be implemented
    >>;
	

template<int size, int round>
pack<int16_t, 8> divide(pack<int16_t, 8> a, pack<int16_t, 8> b)
	
}
#undef PACK_TYPE
