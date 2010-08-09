/*
replay
Software Library

Copyright (c) 2010 Marius Elvert

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.

*/

#ifndef replay_minimal_sphere_hpp
#define replay_minimal_sphere_hpp

#include <list>

namespace replay {

/** Incrementaly construct a d-dimensional point that is equidistant to all
	input points. The input points are on the boundary of a d-dimensional sphere.
	This data structure is numerically robust and will reject point pushes that
	degenerate the numerical stability (i.e. that are very close to the existing ball).
	The code is based on the paper 'Fast and Robust Smallest Enclosing Balls' by
	Bernd Gaertner.
	\ingroup Math
*/
template <class RealType, std::size_t d>
class equisphere
{
public:
	/** Initialize the solver with an error boundary. 1e-15 is a good value to use with floats.
	*/
	equisphere( RealType epsilon ) :
	  m(0), epsilon(epsilon)
	{
		sqr_radius[0] = RealType(-1);
		std::fill_n(center[0],d,RealType(0));
	}

	/** Get the center of the equisphere.
	*/
	const RealType* get_center() const
	{
		return ( m > 0 ) ? center[m-1] : center[0];
	}

	/** Get the squared radius of the sphere that is currently equidistant to all constraint points.
	*/
	RealType get_squared_radius() const
	{
		return ( m > 0 ) ? sqr_radius[m-1] : sqr_radius[0];
	}

	/** Get the number of points currently used as constraints.
	*/
	std::size_t get_support_count() const
	{
		return m;
	}

	/** Remove the last point constraint.
	*/
	void pop()
	{
		--m;
	}

	/** Add a point constraint.
	*/
	template<class VectorType>
	bool push( const VectorType& p )
	{
		// trivial solution
		if ( m==0 )
		{
			for ( std::size_t i=0; i<d; ++i )
				initial_point[i]=center[0][i]=p[i];
		
			sqr_radius[0]=RealType(0);
		}
		else
		{
			using replay::math::square;
			// Compute Q_m=p_i-p_0
			RealType Q_m[d];

			for ( std::size_t i=0; i<d; ++i )
				Q_m[i]=p[i]-initial_point[i];

			// Compute a new last column in a  = Q_B_(m-1)*Q_m
			for (std::size_t i=0; i<m-1; ++i)
			{
				float& lhs = a(i,m-1);
				lhs=RealType(0);

				for (std::size_t j=0; j<d; ++j)
					lhs+=P[i][j]*Q_m[j];

				lhs*=(RealType(2.0)/z[i]);
			}

			// Compute \bar{Q}_m and substract from Q_m
			for (std::size_t i=0; i<m-1; ++i)
			{
				for(std::size_t j=0; j<d; ++j)
					Q_m[j] -= a(i,m-1)*P[i][j];
			}

			// Copy to P
			std::copy(Q_m,Q_m+d,P[m-1]);

			// Compute z_m
			z[m-1]=RealType(0);
			for (std::size_t i=0; i<d; ++i)
				z[m-1] += square(Q_m[i]);
			z[m-1]*=RealType(2);

			// Accept this?
			if (z[m-1]<epsilon*sqr_radius[m-1])
				return false;

			RealType e=-sqr_radius[m-1];
			for (std::size_t i=0; i<d; ++i)
				e+=square(p[i]-center[m-1][i]);
			RealType cf=f[m-1]=e/z[m-1];

			for (std::size_t i=0; i<d; ++i)
				center[m][i] = center[m-1][i]+cf*Q_m[i];
			sqr_radius[m] = sqr_radius[m-1]+e*cf/RealType(2);
		}

		++m;
		return true;
	}

private:
	// (row_size)x(row_size) matrix that has 1 elements on the diagonal, and 0s below the diagonal
	template<std::size_t row_size>
	class upper_unitriangular_matrix
	{
	public:
		RealType operator()(std::size_t r, std::size_t c) const
		{
			if ( r>c )
				return RealType(0);
			else if ( r==c )
				return RealType(1);

			std::size_t skip = (r+1)*(r+2)/2;
			return v[r*row_size+c-skip];
		}

		RealType& operator()(std::size_t r, std::size_t c)
		{
			if ( r>=c )
				throw std::runtime_error( "Cannot write to implicitly defined entries." );

			std::size_t skip = (r+1)*(r+2)/2;
			return v[r*row_size+c-skip];
		}
	private:
		RealType v[((row_size+1)*row_size)/2];
	};

	typedef upper_unitriangular_matrix<d+1> matrix_type;

	RealType		sqr_radius[d+1];
	RealType		center[d+1][d];

	// first point to be inserted, origin of relative coordinate system
	RealType		initial_point[d];

	// store the difference between Q_i and \bar{Q}_i =: P_i
	RealType		P[d][d];

	RealType		z[d];
	RealType		f[d];

	matrix_type		a;

	std::size_t		m;
	const RealType	epsilon;
};


/** Implementation of Welzl's randomized minimal ball algorithm.
	The code is based on the paper 'Fast and Robust Smallest Enclosing Balls' by
	Bernd Gaertner.
	It employs the move-to-front heuristic. However, this heuristic
	is only 'cheap' for std::list containers - otherwise std::rotate is used.
	\ingroup Math
*/
template <class RealType, class VectorType, std::size_t d>
class minimal_ball
{
public:
	/** Generate a minimal bounding ball.
	*/
	template<class ContainerType>
	minimal_ball( ContainerType& p, RealType epsilon )
	{
		// clear the output data
		for (std::size_t i=0;i<d;++i)
			m_center[i]=RealType(0);
		m_square_radius=RealType(0);

		// create a solver
		equisphere<RealType,d> B(epsilon);

		// generate the sphere
		mft_ball(p,p.end(),B);
	}

	/** Test whether the given point is contained in this sphere.
	*/
	bool		contains( const VectorType& v )
	{
		using replay::math::square;

		RealType delta=-m_square_radius;

		for ( std::size_t j=0; j<d; ++j )
			delta+=square(m_center[j]-v[j]);

		return delta <= RealType(0);
	}

	/** Get the center of the minimal ball.
	*/
	const VectorType&	center() const
	{
		return m_center;
	}

	/** Get the square radius of the minimal ball.
	*/
	RealType			square_radius() const
	{
		return m_square_radius;
	}

private:
	
	template<class T>
	static void		move_to_front(
		std::list<T>& c, typename std::list<T>::iterator i )
	{
		c.splice( c.begin(), c, i );
	}

	template<class ContainerType>
	static void		move_to_front(
		ContainerType& c, typename ContainerType::iterator i )
	{
		std::rotate( c.begin(), i, i+1 );
	}

	// copy center and square radius from the solver
	void			update( const equisphere<RealType,d>& B )
	{
		const RealType* src=B.get_center();

		for ( std::size_t i=0;i<d;++i )
			m_center[i] = src[i];

		m_square_radius = B.get_squared_radius();
	}


	template <class ContainerType>
	void mft_ball(
		ContainerType& p, typename ContainerType::iterator e,
		equisphere<float,d>& B )
	{
		typedef typename ContainerType::iterator iterator;

		// finished yet?
		if ( B.get_support_count() == d+1 )
		{
			update(B);
			return;
		}

		// Test all points for inclusion
		for ( iterator i=p.begin(); i!=e; )
		{
			// move ahead before potentially moving this around
			iterator current=i++; 

			// restart with this point in if it wasn't contained
			// and a push is valid
			if ( !contains(*current) && B.push(*current) )
			{
				// each successful push gives us a new ball
				update(B);

				mft_ball(p,current,B);
				B.pop();

				move_to_front(p,current);
			}
		}
	}

	VectorType	m_center;
	RealType	m_square_radius;
};

}

#endif // replay_minimal_sphere_hpp
