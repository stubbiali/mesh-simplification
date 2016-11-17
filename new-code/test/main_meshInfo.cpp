/*!	\file	main_meshInfo.hpp
	\brief	A small executable testing some functionality of class meshInfo. */
	
#include <iostream>
#include <chrono>

#include "meshInfo.hpp"

int main()
{
	using namespace geometry;
	using namespace std::chrono;
	
	high_resolution_clock::time_point start, stop; 
	
	start = high_resolution_clock::now();
	
	// Import mesh
	string inputfile("../../mesh/bunny.inp");
	
	// Build connections
	meshInfo<Triangle, MeshType::DATA> news(inputfile);
	
	stop = high_resolution_clock::now();
	auto duration_read = duration_cast<milliseconds>(stop-start).count();
	
	//
	// Edge (6721,16057)
	//
	// Elements connected to 6721: 886,888,1119,8034,8194,8356
	// Elements connected to 16057: 1552,1833,1983,5775,8034,8194
	
	//
	// Test results
	//
	
	{
		UInt id1(6721), id2(16057);
		auto invElems = news.getElemsInvolvedInEdgeCollapsing(id1,id2);
		auto toRemove = news.getElemsOnEdge(id1,id2);
		auto toKeep = news.getElemsModifiedInEdgeCollapsing(id1,id2);
		auto invData = news.getDataInvolvedInEdgeCollapsing(id1,id2);
		auto toMove = news.getDataModifiedInEdgeCollapsing(id1,id2);
		
		cout << "invElems = { ";
		for (auto el : invElems)
			cout << el << " ";
		cout << "}" << endl;
	
		cout << "toRemove = { ";
		for (auto el : toRemove)
			cout << el << " ";
		cout << "}" << endl;
	
		cout << "toKeep   = { ";
		for (auto el : toKeep)
			cout << el << " ";
		cout << "}" << endl;
	
		cout << "invData  = { ";
		for (auto el : invData)
			cout << el << " ";
		cout << "}" << endl;
	
		cout << "toMove   = { ";
		for (auto el : toMove)
			cout << el << " ";
		cout << "}" << endl << endl;
	}
	
	//
	// Test performance
	//
	
	UInt times(1e5);
	start = high_resolution_clock::now();
	for (UInt i = 0; i < times; i++)
	{
		UInt id1(6721), id2(16057);
		auto invElems = news.getElemsInvolvedInEdgeCollapsing(id1,id2);
		auto toRemove = news.getElemsOnEdge(id1,id2);
		auto toKeep = news.getElemsModifiedInEdgeCollapsing(id1,id2);
		//auto invData = news.getDataInvolvedInEdgeCollapsing(id1,id2);
		//auto invData = news.getDataInvolvedInEdgeCollapsing(invElems);
		//auto toMove = news.getDataModifiedInEdgeCollapsing(id1,id2);
		auto toMove = news.getDataModifiedInEdgeCollapsing(invElems);
	}
	stop = high_resolution_clock::now();
	auto duration_news = duration_cast<milliseconds>(stop-start).count();
	
	cout << "Time for reading the mesh: " << duration_read << " ms" << endl
		 << "Time for extracting info : " << duration_news << " ms" << endl;
}


