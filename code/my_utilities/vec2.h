///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains source code for my 2D vector class
 *	\file		vec2.h
 *	\author		Fish
 *	\date		July 17, 2011
 *  \brief      Not the fastest, but it's mine
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef VEC2F_H_
#define VEC2F_H_

// Headers
#include <math.h>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const float pi = atan(1.0f) * 4.0f;

inline float RADTODEG(const float Radians)
{
    return Radians * 180.0f / pi;
}
inline float DEGTORAD(const float Degrees)
{
    return Degrees * pi / 180.0f;
}


    template <class T>
    class Vec2
    {
        public:

        //! Plain constructor
        inline                                  Vec2<T>() {}

        //! Copy ctor
        inline                                  Vec2<T>(const Vec2<T>& v) : x(v.x), y(v.y)      {}

        //! Constructor taking x, y arguments
        inline                                  Vec2<T>(const T x0, const T y0) : x(x0), y(y0)  {}

        //! Zero a vector
                void                            Zero()      { this->x = this->y = 0;                              }

        //! Boolean equivalence compare vector
        inline bool operator==(const Vec2<T>& v) const      { return (x == v.x && y == v.y);                      }

        //boolean non-equivalence compare vector
        inline bool operator!=(const Vec2<T>& v)            { return !(this == v);                                }

        //assignment operator
        inline const Vec2<T>& operator=(const Vec2<T>& v)   { x = v.x;   y = v.y;    return *this;                }

        //subtract vector
        inline const Vec2<T>& operator-=(const Vec2<T>& v)  { x -= v.x;y -= v.y;return *this;                     }


        /// Format Rest Later... ///

        //add vector
        inline const Vec2<T>& operator+=(const Vec2<T>& v)
        {
            x += v.x;
            y += v.y;
            return *this;
        }
        //scale vector (multiply)
        inline const Vec2<T>& operator*=(const T num)
        {
            x *= num;
            y *= num;
            return *this;
        }

        //scale vector (divide)
        inline const Vec2<T>& operator/=(const T num)
        {
            x /= num;
            y /= num;
            return *this;
        }

        //negation of vector
        inline const Vec2<T> operator-() const
        {
            return Vec2<T>(-x, -y);
        }

        //subtract vector
        inline const Vec2<T> operator-(const Vec2<T>& v) const
        {
            Vec2<T> temp(*this);
            temp.x -= v.x;
            temp.y -= v.y;
            return temp;
        }

        //add vector
        inline const Vec2<T> operator+(const Vec2<T>& v) const
        {
            Vec2<T> temp(*this);
            temp.x += v.x;
            temp.y += v.y;
            return temp;
        }

        //scale vector (mulitply); form: Vec * float
        inline const Vec2<T> operator*(const T num) const
        {
            Vec2<T> temp(*this);
            temp.x *= num;
            temp.y *= num;

            return temp;
        }

        //scale vector (multiply); form: float * Vec
        friend const Vec2<T> operator*(const T num, Vec2<T> v)
        {
            v.x *= num;
            v.y *= num;
            return v; //value of v is unchanged
        }
        //scale vector (divide)
        inline const Vec2<T> operator/(const T num) const
        {
            Vec2<T> temp(*this);
            temp.x /= num;
            temp.y /= num;
            return temp;
        }

        //dot product
        inline T operator*(const Vec2<T>& v) const
        {
            return (x * v.x + y * v.y);
        }

        //cross product
        inline float operator ^(const Vec2<T>& v) const
        {
            return ((x * v.y) - (y * v.x));
        }

            // horizontal and vertical components
            T x, y;
    };

    //macros for creating ctors for derived Vectors
    #define VECTOR2_CTORS(name, T)                        \
            name() {}                                     \
            name(const Vec2<T>& v) : Vec2<T>(v.x, v.y) {} \
            name(T x0, T y0) : Vec2<T>(x0, y0) {}


    class Vec2i : public Vec2<int>
    {
    public:
        inline VECTOR2_CTORS(Vec2i, int);
    };

    class Vec2f : public Vec2<float>
    {
    public:
        inline VECTOR2_CTORS(Vec2f, float);

        //gets squared length of the vector (fast) (Euclidean)
        inline float LengthSquared() const
        {
            return (float)(*this * *this);
        }

        //gets length of vector (Euclidean)
        inline float Length() const
        {
            return (float)sqrt(*this * *this);
        }

        inline void Rotate(float rad)
        {
            float tx = x;
            x = x * cos(rad) - y * sin(rad);
            y = tx * sin(rad) + y * cos(rad);
        }

        //get value of rotated radian
        inline Vec2f Rotated(float rad) const
        {
            float x1 = x * cos(rad) - y * sin(rad);
            float y1 = x * sin(rad) + y * cos(rad);

            return Vec2f(x1, y1);
        }

        //shift and rotate
        inline void Transform(Vec2f pos, float rad)
        {
            Vec2f tempV = *this;

            this->x = tempV.x * cos(rad) - tempV.y * sin(rad) + pos.x;
            this->y = tempV.x * sin(rad) + tempV.y * cos(rad) + pos.y;
        }

        //shift and rotate
        inline Vec2f Transformed(Vec2f pos, float rad) const
        {
            Vec2f tempV;

            tempV.x = this->x * cos(rad) - this->y * sin(rad) + pos.x;
            tempV.y = this->x * sin(rad) + this->y * cos(rad) + pos.y;

            return tempV;
        }

        inline Vec2f GetNormal() const
        {
            return Vec2f(-y, x);
        }

        //normalize the vector
        void Normalize()
        {
            *this /= Length();
        }

        //returns normalized vector
        Vec2f Normalized() const
        {
            return *this / Length();
        }

        //reflect vector about n
        void Reflect(const Vec2f& n) //not sure if this is right...
        {
            Vec2f orig(*this);
            Project(n);
            *this = *this * 2 - orig;
        }

        //projects vector onto v
        void Project(const Vec2f& v)
        {
            *this = v * (*this * v)/(v * v);
        }

        //returns projected vector onto v
        Vec2f Projected(const Vec2f& v)
        {
            Vec2f temp(*this);
            return v * (temp * v)/(v * v);
        }

        //projects vector onto v
        void PerpProject(const Vec2f& v)
        {
            *this = *this - v * (*this * v)/(v * v);
        }

        //returns projected vector onto v
        Vec2f PerpProjected(const Vec2f& v)
        {
            Vec2f temp(*this);
            return temp - v * (temp * v)/(v * v);
        }

        //calculates radian between 2 vectors (in rad)
        float Angle(const Vec2f& v2)
        {
            return acosf((*this*v2)/(this->Length() * v2.Length()));
        }

        //calculates radian between 2 normalized vectors (in rad)
        float AngleNormalized(const Vec2f& v2)
        {
            return acosf(*this*v2);
        }

        float Angle2(const Vec2f& v2)
        {
            float result = atan2(v2.y, v2.x) - atan2(y, x);
            if (fabs(result) > 2.f*pi)
                return fmod(result, 2.f*pi);
            return result;
        }

        //calculates radian between 2 normalized vectors (in rad)
        float AngleNormalized2(const Vec2f& v2)
        {
            return acosf(*this*v2);
        }




    };


#endif //VEC2F_H_
