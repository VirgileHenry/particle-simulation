#pragma once

#include "visualizer.hpp"
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

template<typename Universe>
class XMLVisualizer : public Visualizer<Universe> {

    private:
    string fileName;
    int nbPoints;
    int nbCells;
    int nbDimensions;
    int nbIteration = 0;

    public:
    XMLVisualizer(int nbParticles, int nbChunks, int nbDimensions, string globalFileName){
        this->nbPoints = nbParticles;
        this->nbCells = nbChunks;
        this->nbDimensions = nbDimensions;
        this->fileName = globalFileName;
    }


    /// @brief Write all the datas in an XML file
    void draw(Universe* universe) override{

        ofstream myFlow((this->fileName+to_string(this->nbIteration)+".vtu").c_str());

        if (myFlow){

            myFlow << "<VTKFile type=\"UnstructuredGrid\" version=\"0.1\" byte_order=\"BigEndian\">" << endl;
            myFlow << "<UnstructuredGrid>" << endl;
            myFlow << "<Piece NumberOfPoints=\"" << this->nbPoints <<"\" NumberOfCells=\"" << this->nbCells <<"\">" << endl;
            myFlow << "<Points>" << endl;
            myFlow << "<DataArray name=\"Position\" type=\"Float32\" NumberOfComponents=\"" << 3 << "\" format=\"ascii\">" << endl;
            for(auto particle : universe->getParticles()){
                for(int dimension=0; dimension<3; dimension++){
                    if(dimension<this->nbDimensions){
                        myFlow << particle.getPosition()[dimension];
                        myFlow << " ";
                    }
                    else{
                        myFlow << 0;
                        myFlow << " ";
                    }
                }
            }
            myFlow << endl;
            myFlow << "</DataArray>" << endl;
            myFlow << "</Points>" << endl;
            myFlow << "<PointData Vectors=\"vector\">" << endl;
            myFlow << "<DataArray type=\"Float32\" Name=\"Velocity\" NumberOfComponents=\"" << 3 << "\" format=\"ascii\">" << endl;
            for(auto particle : universe->getParticles()){
                for(int dimension=0; dimension<3; dimension++){
                    if(dimension< this->nbDimensions) {
                        myFlow << particle.getVelocity()[dimension];
                        myFlow << " ";
                    }
                    else{
                        myFlow << 0;
                        myFlow << " ";
                    }
                }
            }
            myFlow << endl;
            myFlow << "</DataArray>" << endl;
            myFlow << "<DataArray type=\"Float32\" Name=\"Masse\" format=\"ascii\">" << endl;
            for(auto particle : universe->getParticles()){
                myFlow << particle.getMass();
                myFlow << " ";
            }
            myFlow << endl;
            myFlow << "</DataArray>" << endl;
            myFlow << "</PointData>" << endl;
            myFlow << "<Cells>" << endl;
            myFlow << "<DataArray type=\"Int32\" Name=\"connectivity\" format=\"ascii\">" << endl;
            myFlow << "</DataArray>" << endl;
            myFlow << "<DataArray type=\"Int32\" Name=\"offsets\" format=\"ascii\">" << endl;
            myFlow << "</DataArray>" << endl;
            myFlow << "<DataArray type=\"UInt8\" Name=\"types\" format=\"ascii\">" << endl;
            myFlow << "</DataArray>" << endl;
            myFlow << "</Cells>" << endl;
            myFlow << "</Piece>" << endl;
            myFlow << "</UnstructuredGrid>" << endl;
            myFlow << "</VTKFile>" << endl;

        }
        else{
            cout << "ERROR : The file can not be open." << endl;
        }
        this->nbIteration++;
    }
};