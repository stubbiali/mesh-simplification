/*!	\file	inline_DataGeo.hpp
	\brief	Implementations of inlined members of class DataGeo. */
	
#ifndef HH_INLINEDATAGEO_HH
#define HH_INLINEDATAGEO_HH

namespace geometry
{
	//
	// Access members
	//
	
	INLINE array<Real,10> DataGeo::getQMatrix(const UInt & id) const
	{
		return Qs[id];
	}
	
	
	INLINE vector<array<Real,10>> DataGeo::getQs() const
	{
		return Qs;
	}
	
	
	INLINE point3d DataGeo::getOriginalDataPointLocation(const UInt & id) const
	{
		return dataOrigin[id];
	}
	
	
	INLINE Real DataGeo::getQuantityOfInformation(const UInt & id) const
	{
		return qoi[id];
	}
	
	
	INLINE UInt DataGeo::getNumElems() const
	{
		return numElems;
	}
	
	
	INLINE Real DataGeo::getAverageQuantityOfInformation() const
	{
		return qoi_mean;
	}
	
	
	INLINE Real DataGeo::getMaxCost(const UInt & i) const
	{
		return maxCost[i];
	}
	
	
	INLINE Real DataGeo::getWeight(const UInt & i) const
	{
		return weight[i];
	}
	
	
	INLINE void DataGeo::setWeight(const UInt & i, const Real & val)
	{
		weight[i] = val;
	}
}

#endif
