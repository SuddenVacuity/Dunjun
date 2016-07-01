#ifndef DUNJUN_SYSTEM_TYPES_HPP
#define DUNJUN_SYSTEM_TYPES_HPP

#include <Dunjun/System/Config.hpp>

#include <cstddef>	// added for s16/f16 type structs at end of file
#include <cstring>	// added for s16/f16 type structs at end of file
#include <memory>	// added for s16/f16 type structs at end of file

#define GLOBAL static // set all these types to refer to static
#define INTERNAL static
#define LOCAL_PERSIST static

namespace Dunjun
{
	using s8  = signed char;
	using u8  = unsigned char;
	using s16 = signed short;
	using u16 = unsigned short;
	using s32 = signed int;
	using u32 = unsigned int;

#if defined(_MSC_VER)
	using s64 = signed __int64;
	using u64 = unsigned __int64;
#else
	using s64 = signed long long;
	using u64 = unsigned long long;
#endif

	using f32 = float;
	using f64 = double;

	using b8 = bool;
	using b32 = s32; // define this as 32 bit for it to be dword alignde

	using usize = std::size_t;
	using uintptr = uintptr_t;
	using intptr = intptr_t;

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
)				.
)					.
)
)				PAST HERE WAS JUST COPIED AND HASN"T BEEN LOOKED THROUGH
)					.
)
)				.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/


	namespace
		{
		// TODO(bill): May not be complete - Handling NaN/+-inf
			inline s16 f32Tof16(f32 f)
			 {
			s16 fint16;
			s32 fint32;
			std::memcpy(&fint32, &f, sizeof(f32));
			fint16 = ((fint32 & 0x7fffffff) >> 13) - (0x38000000 >> 13);
			fint16 |= ((fint32 & 0x80000000) >> 16);
			
				return fint16;
			}
		
			// TODO(bill): May not be complete - Handling NaN/+-inf
			inline f32 f16Tof32(s16 fint16)
			 {
			s32 fint32 = ((fint16 & 0x8000) << 16);
			fint32 |= ((fint16 & 0x7fff) << 13) + 0x38000000;
			
				f32 fRet;
			std::memcpy(&fRet, &fint32, sizeof(f32));
			return fRet;
			}
		} // namespace (anonymous)
	
		class f16
		 {
		public:
			f16()
				 : m_value{ 0 }
				{
				}
			
				f16(f32 f)
				 : m_value{ f32Tof16(f) }
				{
				}

			f16(const f16& f) = default;
			f16(f16&& f)
				 : m_value{ std::move(f.m_value) }
				{
				}
			
				operator f32() { return f16Tof32(m_value); }
			operator f32() const { return f16Tof32(m_value); }
			
				f16& operator=(const f16& f) { m_value = f; return *this; }
			f16& operator+=(const f16& f)
				 {
				*this = *this + f;
				return *this;
				}
			f16& operator-=(const f16& f)
				 {
				*this = *this - f;
				return *this;
				}
			f16& operator*=(const f16& f)
				 {
				*this = *this * f;
				return *this;
				}
			f16& operator/=(const f16& f)
				 {
				*this = *this / f;
				return *this;
				}
			f16& operator-()
				 {
				*this = f16{ -static_cast<f32>(*this) };
				return *this;
				}
			
				friend f16 operator+(const f16& a, const f16& b);
			friend f16 operator-(const f16& a, const f16& b);
			friend f16 operator*(const f16& a, const f16& b);
			friend f16 operator/(const f16& a, const f16& b);
			
				protected:
					s16 m_value;
					};
	
		inline f16 operator+(const f16& a, const f16& b)
		 {
		return{ static_cast<f32>(a) + static_cast<f32>(b) };
		}
	inline f16 operator-(const f16& a, const f16& b)
		 {
		return{ static_cast<f32>(a) - static_cast<f32>(b) };
		}
	inline f16 operator*(const f16& a, const f16& b)
		 {
		return{ static_cast<f32>(a) * static_cast<f32>(b) };
		}
	inline f16 operator/(const f16& a, const f16& b)
		 {
		return{ static_cast<f32>(a) / static_cast<f32>(b) };
		}



}


#endif
