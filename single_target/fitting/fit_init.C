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
#include <vector>

void fit_init()
{
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
				if (j==0)
				{
					for (int l=0;l<param_num[k];l++)
					{
						params[k][l].push_back(gROOT->ProcessLine(Form("param%i(%i);",l+1,i)));
					}
				}
			}
		}
	}
	vector<double> params_nodups[files][maxparam]; // two-dimensional array of parameter vectors, this time with no duplicates
	int max_param_options=1;
	for (int i=0;i<files;i++)
	{
		cout <<"Number of possibilities for path " <<paths[i] <<": " <<entries[i] <<endl;
		for (int j=0;j<param_num[i];j++)
		{
			params_nodups[i][j] = removeDuplicates(params[i][j]);
			cout <<"\t Parameter " <<j <<" has " <<params_nodups[i][j].size() <<" unique value(s)." <<endl;
			if (max_param_options<params_nodups[i][j].size())
			{
				max_param_options = int(params_nodups[i][j].size());
			}
		}
	}

	vector<double> tempvec;
	vector<double> state_params[16]; // for each state parameter, a list of the unique possible values. has already checked that all possible path combinaionts will satisfy every unique value.
	apply_states();

	//figure out what the parameter space is
	for (int i=0;i<files;i++)
	{
		for (int j=0;j<param_num[i];j++)
		{
			if (state_params[states[i][j]].size()==0)
			{
				state_params[states[i][j]] = params_nodups[i][j];
			}
			else
			{
				tempvec = vecAND(state_params[states[i][j]],params_nodups[i][j]);
				state_params[states[i][j]] = tempvec;
			}
		}
	}

	vector<int> indices_sorted[files][maxparam][max_param_options]; // array to hold the indices that satisfy the various unique state parameters
	for (int k=0; k<files; k++)
	{
		for (int j=0;j<param_num[k];j++)
		{
			for (int i=0; i<state_params[states[k][j]].size();i++)
			{
				for (int l=0;l<entries[k];l++)
				if (state_params[states[k][j]][i]==params[k][j][l]) indices_sorted[k][j][i].push_back(l);
			}
		}
	}

	Looper loops;
	out.open(Form("output/10li_%icomponents_indices.dat",components),std::ofstream::out);

	for (int i=0;i<16;i++)
	{
		loops.AddLoop(state_params[i].size());
	}

	int counter = 0;
	vector<int> iteration,result,temp1,temp2,path_iteration;
	while(!loops.Finished())
	{
		counter++;
		path_iteration.clear();
		iteration=loops.GetNext();
		for (int i=0;i<files;i++)
		{
			result = indices_sorted[i][0][iteration[states[i][0]]];
			for (int j=1;j<param_num[i];j++)
			{
				temp1 = indices_sorted[i][j][iteration[states[i][j]]];
				temp2 = vecAND(temp1,result);
				result = temp2;
			}
			path_iteration.push_back(result[0]);
		}
		for (int i=0;i<files-1;i++)
		{
			out <<path_iteration[i] <<"\t";
		}
		out <<path_iteration[files-1] <<endl;
	}
	out.close();
	cout <<"Total number of state parameter permutations: " <<counter <<endl;
}
