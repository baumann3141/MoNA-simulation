#include "TH2D.h"
#include "TF2.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TRandom3.h"
#include "TVirtualFitter.h"
#include "TList.h"
#include "TFile.h"
#include "TROOT.h"
#include "TMath.h"
#include "TSystem.h"
#include <iostream>
#include <fstream>
#include <Fitter.C>

void run_fit(int part,int parts){

	//declarations
	int entries[files];// number of histograms for each path
	int param_num[files]; // number of parameters for each path
	TObjArray* names = new TObjArray(files); // histogram name vectors for each path
	TObjArray* hsts = new TObjArray(files); // histogram object array
	vector<double> params[files][maxparam]; // two-dimensional array of parameter vectors
	TFile *f[files]; // array of files to be opened
	int location,size,i2;
	string tester, tester2;
	vector<string> hst_names;
	const char* name1;

	//loop over paths
	for (int k=0;k<files;k++)
	{
		//open path file
		f[k] = TFile::Open(Form("path%i/path%i.root",paths[k],paths[k]));

		//list of all objects in ROOT file
		TList* list = gFile->GetListOfKeys();
		entries[k] = list->GetSize();

		//initialize param_num
		param_num[k] = 0;

		for (int i=0;i<entries[k];i++)
		{
			tester = list->At(i)->GetName();
			location = tester.find("_1n_");
			if (location>100 || location<0) param_num[k]++;
			else if (k==0)
			{
				tester2 = tester.substr(0,location);
				size = hst_names.size();
				if (size==0) hst_names.push_back(tester2);
				else
				{
					if (hst_names[size-1].compare(tester2)==0)
					{
						//do nothing
					}
					else
					{
						hst_names.push_back(tester2);
					}
				}
			}
		}

		//set number of histograms (iterations with different parameter values, not types of histograms)
		entries[k] = (entries[k] - param_num[k])/hst_names.size();

		//TObjArray for histograms from this file
		TObjArray* hst1 = new TObjArray(hst_names.size());
		hsts->AddAt(hst1,k);
		names->AddAt(new TObjArray(hst_names.size()),k);

		//loop over entries to make pointers to histograms
		//and to create parameter array
		for (int j=0;j<hst_names.size();j++)
		{
			((TObjArray*)hsts->At(k))->AddAt(new TObjArray(entries[k]),j);
			((TObjArray*)names->At(k))->AddAt(new TObjArray(entries[k]),j);
			for (int i=0;i<entries[k];i++)
			{
				i2 = entries[k]*j + i;
				name1 = list->At(i2)->GetName();
				((TObjArray*)((TObjArray*)names->At(k))->At(j))->AddAt(new TObjString(name1),i);
				((TObjArray*)((TObjArray*)hsts->At(k))->At(j))->AddAt(gFile->Get(name1),i);
				for (int l=0;l<param_num[k];l++)
				{
					if (j==0)
					{
						params[k][l].push_back(gROOT->ProcessLine(Form("param%i(%i);",l+1,i)));
					}
				}
			}
		}
	}
	for (int i=0;i<files;i++)
	{
		cout <<"Number of possibilities for path " <<paths[i] <<": " <<entries[i] <<endl;
	}

	//open data file and assign pointers.
	TFile *fdata = TFile::Open(datafile);
	TObjArray* data_array = new TObjArray(hst_names.size());
	TH1F* temp = (TH1F*) gFile->Get("edecay_data");
	data_array->AddAt(temp,0);
	// 10Be edit
//	temp = (TH1F*) gFile->Get("vrel_data");
//	data_array->AddAt(temp,1);

	// read in indices file
	ifstream in;
	in.open(Form("output/10li_%icomponents_indices.dat",files),std::ifstream::in);
	float index;
	int index_int;
	vector<int> input;
	vector< vector<int> > indices;
	int line=0;
	in >> index;
	while (in.good())
	{
		input.clear();
		for (int i=0;i<files;i++)
		{
			input.push_back(int(index));
			in >> index;
		}
		indices.push_back(input);
		line++;
	}
	in.close();

	cout <<"Start fitting." <<endl;
	//loop over possible simulation combinations
	fitting_from_file(hsts,data_array,params,param_num,indices,part,parts);
	cout <<"Done fitting." <<endl;
}
