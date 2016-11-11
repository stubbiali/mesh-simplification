/*! \file	mesh.hpp
	\brief	A class storing a mesh and possible distributed data. */
	
#ifndef HH_MESH_HH
#define HH_MESH_HH

#include "bmesh.hpp"
#include "dataPoint.hpp"

namespace geometry
{
	/*! Mesh type. */
	enum class MeshType {GEO, DATA};
	
	/*! This class represents a wrapper for bmesh, which it inherits.
		It takes two template parameters:
		<ol>
		<li> SHAPE: shape of the elements;
		<li> MT: 	mesh type, i.e. with or without distributed data;
					default is GEO.
		<\ol>
		When MT is set to GEO (i.e. no data): the class just inherits bmesh
		without adding or overriding members.
		When MT is set to DATA (i.e. with data): the class inherits bmesh and 
		adds members for handling data distributed over the mesh.
		
		Note: no methods printing in a formatted way a mesh with distributed 
		data implemented yet. Think about it!
		
		\sa bmesh, MeshType */
	template<typename SHAPE, MeshType MT = MeshType::GEO>
	class mesh final : public bmesh<SHAPE>
	{
	};
	
	/*! Partial specialization for grids with distributed data. */
	template<typename SHAPE>
	class mesh<SHAPE, MeshType::DATA> final : public bmesh<SHAPE>
	{
		private:
			/*! List of data. */
			vector<dataPoint> data;
			
		public:
			//
			// Constructors
			//
			// For the moment, if the user does not specify the data locations 
			// and/or values we suppose they coincide with the nodes and they 
			// are all equal to zero (or randomly generated).
			
			/*! Constructor.
				\param nds	vector of nodes
				\param els	vector of elements
				\param val	vector of data values */
			mesh(const vector<point> & nds, 
				const vector<geoElement<SHAPE>> & els,
				vector<Real> val);
				
			/*! Constructor.
				\param nds	vector of nodes
				\param els	vector of elements
				\param dat	vector of data points */
			mesh(const vector<point> & nds, 
				const vector<geoElement<SHAPE>> & els,
				const vector<dataPoint> & dat = vector<dataPoint>());
					
			/*! Constructor.
				\param filename	name of input file */
			mesh(const string & filename);
			
			/*! Synthetic copy constructor.
				\param m	another mesh */
			mesh(const mesh<SHAPE, MeshType::DATA> & m) = default;
			
			//
			// Operators
			//
			
			/*! Output stream operator.
				\param out	output stream
				\param m	mesh
				\return		updated output stream */
			template<typename S>
			friend ostream & operator<<(ostream & out, const mesh<S, MeshType::DATA> & m);
			
			//
			// Get methods
			//
			
			/*! Get a data point.
				\param Id	point Id
				\return		the data point */
			dataPoint getData(const UInt & Id) const;
			
			/*! Get number of data point.
				\return		number of data */
			UInt getNumData() const;
			
			//
			// Set methods
			//
			
			/*! Set location of a data point.
				\param Id	point Id
				\param coor	array with new coordinates */
			void setData(const UInt & Id, const array<Real,3> & coor);
			
			/*! Set associated value of a data point. 
				\param Id	point Id
				\param val	new value */
			void setData(const UInt & Id, const Real & val);
			
			/*! Set location and associated value of a data point.
				\param Id	point Id
				\param coor	array with new coordinates
				\param val	new value */
			void setData(const UInt & Id, const array<Real,3> & coor, const Real & val);
			
			/*! Resize vector of data.
				\param numData	new number of data */
			void resizeData(const UInt & numData);
			
			/*! Make a reserve for the vector of data.
				\param numData	new number of data */
			void reserveData(const UInt & numData);
			
			//
			// Insert, erase and clear methods
			//
			
			/*! Insert a new data point at the end of the list.
				\param coor	point coordinates
				\param val	point data */
			void insertData(const array<Real,3> & coor, const Real & val = 0);
			
			/*! Remove a data point from the list.
				\param Id	point Id */
			void eraseData(const UInt & Id);
			
			/*! Clear the lists. */
			virtual void clear();
			
		protected:
			//
			// Update Id's
			//
			
			/*! Update data points Id's so to make them coincide with the position in the vector. */
			void setUpDataIds();
	};
	
}

/*! Include definitions of members and friend functions. */
#include "implementation/imp_mesh.hpp"

#endif


