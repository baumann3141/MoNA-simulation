{
	gSystem->Load("Fitter_C.so");
	gSystem->Load("fit2dHist_C.so");
	#include<settings.C>
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

	// get actual number of parameters
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

	//get total number of lines
	TString tempstr = gSystem->GetFromPipe(Form("awk 'END{print FNR}' output/10li_%icomponents_chi2.dat",files));
	const int lines = tempstr.Atoi();
	tempstr = gSystem->GetFromPipe(Form("awk 'END{print NF}' output/10li_%icomponents_chi2.dat",files));
	//const int npars = tempstr.Atoi()-1;
	// use line below to look at scale output. - JKS
	int npars;
	if (asymErrors) npars = tempstr.Atoi()-1-files*3;
	else npars = tempstr.Atoi()-1-files;
	vector<int> pars1d; // the parameters we're actually varying

	for (int i=0;i<14;i++)
	{
		if (state_params[i].size()>1) pars1d.push_back(i); 
	}

	cout <<"There are " <<pars1d.size() <<" distinct parameters to observe." <<endl;

	//open chi^2 file
	ifstream in;
	in.open(Form("output/10li_%icomponents_chi2.dat",files));

	float chi_array[lines];
	int param_array[npars][lines];
	double scale_array[files][lines];
	double err_array[files][2][lines]; // two for plus and minus error
	float chi2, param;
	int minindex=0;
	float minchi2=100e4;

	// read chi^2 file
	for (int i=0;i<lines;i++)
	{
		in >> chi_array[i];
		for (int j=0;j<npars;j++)
		{
			in >> param_array[j][i];
		}
		// use loop below to read out scales included in chi2 file. - JKS
		for (int j=0;j<files;j++)
		{
			if (asymErrors)
			{
				in >> scale_array[j][i];
				in >> err_array[j][0][i];
				in >> err_array[j][1][i];
			}
			else	in >> scale_array[j][i];
		}
		if (chi_array[i]<minchi2)
		{
			minindex=i;
			minchi2=chi_array[i];
		}
	}
	cout <<"Minimum index: " <<minindex <<endl;
	cout <<"Minimum chi^2: " <<minchi2 <<endl;

	// close chi^2 file
	in.close();

	//------------------------------------display best fit--------------------------------------//
	
	// pull minimum parameters
	int min_params[npars];
	cout <<"Parameters of minimum fit:" <<endl;
	for (int i=0;i<npars;i++)
	{
		min_params[i]=param_array[i][minindex];
		cout <<"Parameter " <<i <<": " <<min_params[i] <<endl;
	}

	// read in indices file
	in.open(Form("output/10li_%icomponents_indices.dat",files),std::ifstream::in);
	float index;
	int index_int;
	//int input[files];
	vector<float> input;
	//vector< int* > indices;
	vector< vector<float> > indices;
	int line=0;
	in >> index;
	while (in.good())
	{
		input.clear();
		for (int i=0;i<files;i++)
		{
			input.push_back(index);
		//	input[i]=index;
			in >> index;
		}
		indices.push_back(input);
		line++;
	}
	in.close();

	// go through the original indices list to find index of indices that matches the parameters for the minimum chi^2.
	int par_num,min_index;
	bool breakout=kFALSE;
	bool found=kFALSE;
	for (int i=0;i<indices.size();i++)
	{
		par_num = 0;
		breakout=kFALSE;
		for (int j=0;j<files;j++)
		{
			for (int k=0;k<param_num[j];k++)
			{
				if (params[j][k][indices[i][j]]==min_params[par_num])
				{
					par_num++;
				}
				else
				{
					breakout=kTRUE;
					break;
				}
			}
			if (breakout==kTRUE)
			{
				break;
			}
		}
		if (breakout) continue;
		else
		{
			min_index = i;
			found=kTRUE;
			break;
		}
	}
	if (!found)
	{
		cout <<"Error: minimum not found." <<endl;
		cout <<"Exiting early." <<endl;
		return (0);
	}
	cout <<"Minimum index from list of indices: " <<min_index <<endl;
	for (int i=0;i<files;i++)
	{
		cout <<indices[min_index][i] <<"\t";
	}
	cout <<endl;

	//open data file and assign pointers.
	TFile *fdata = TFile::Open(datafile);
	TObjArray* data_array = new TObjArray(hst_names.size());
	temp = (TH1F*) gFile->Get("edecay_data");
	data_array->AddAt(temp,0);
	//10Be edit
/*	temp = (TH1F*) gFile->Get("edecaymult1_data");
	data_array->AddAt(temp,1);
	temp = (TH1F*) gFile->Get("edecay3body_data");
	data_array->AddAt(temp,2);
	temp = (TH1F*) gFile->Get("edecay3bodycaus_data");
	data_array->AddAt(temp,3);
	temp = (TH1F*) gFile->Get("mult_data");
	data_array->AddAt(temp,4);*/
	temp = (TH1F*) gFile->Get("vrel_data");
	data_array->AddAt(temp,1);

	// For best fit, re-do the fit to get the scaling factors
	vector<int> minindices;
	for (int i=0;i<indices[min_index].size();i++)
	{
		minindices.push_back(int(indices[min_index][i]));
	}
	AssignSimHists(hsts,minindices);
	AssignExpHists(data_array);
	fit2dHist_Run(13);

	set_pretty_plots();
	// set up canvas for displaying best fit
	TCanvas *c1 = new TCanvas("c1","Best fit",nx*800,ny*600);
	c1->Divide(nx,ny);
	
	// set up arrays for best fit simulations
	TH1F* sim_array[files][numplots];
	TH1F* simt_array[numplots];

	// fill simulation array with best fit components
	for (int k=0; k<files; k++)
	{
		for (int j=0; j<numplots; j++)
		{
			sim_array[k][j] = (TH1F*) ((TObjArray*)((TObjArray*)hsts->At(k))->At(j))->At(indices[min_index][k]);
		}
	}

	// display data, total and components of best fit
	for (int i=0; i<numplots; i++)
	{
		c1->cd(i+1);
		((TH1F*) data_array->At(i))->Draw("pe1");
		for (int j=0; j<files; j++)
		{
			// scale appropriately
			sim_array[j][i]->Scale(fScale[j]);
			
			// begin creating summed histogram
			if (j==0) simt_array[i] = (TH1F*) sim_array[j][i]->Clone(Form("simtotal%i",i));
			else simt_array[i]->Add(sim_array[j][i]);

			// draw component
			sim_array[j][i]->Draw("samec");
		}
		simt_array[i]->SetLineWidth(2);
		simt_array[i]->SetLineColor(kBlack);
		simt_array[i]->Draw("samec");

		// fix the range so everything is visible
		double simmax, datamax;
		simmax = simt_array[i]->GetMaximum();
		datamax = ((TH1F*) data_array->At(i))->GetMaximum();
		((TH1F*) data_array->At(i))->GetYaxis()->SetRangeUser(0,TMath::Power(max(datamax,simmax),0.5)+max(datamax,simmax)*1.05);
	}

	
	//------------------------------------display 1D chi2 plots--------------------------------------//
	// sort chi^2 array and put ordered indices in indices array
	int sorted_indices[lines];
	TMath::Sort(lines,chi_array,sorted_indices);

	// make one-dimensional chi^2 histograms
	int min,max,bins,bin,points;
	int left = 0;
	double x,y,z;
	double maxz = 0;
	const int npars = pars1d.size();
	TH1F* chi2_hsts[npars];
	TGraph* chi2_1d[npars];
	vector<int> params_2d;
	int param_array_nodups[npars][lines]; //array of parameters with no duplicate parameters

	// zero out param_array_nodups
	for (int i=0;i<npars;i++)
	{
		for (int j=0;j<lines;j++)
		{
			param_array_nodups[i][j]=0;
		}
	}

	int counter=0;
	//reduce param_array to eliminate duplicate parameters
	for (int i=0;i<files;i++)
	{
		for (int j=0;j<param_num[i];j++)
		{
			for (int k=0;k<npars;k++)
			{
				if (states[i][j]==pars1d[k])
				{
					//cout <<"Total parameter " <<counter <<", which is parameter " <<j <<" from file " <<paths[i] <<" is state variable " <<states[i][j] <<endl;
					//cout <<"This matches with " <<pars1d[k] <<endl;
					for (int m=0;m<lines;m++)
					{
						param_array_nodups[k][m] = param_array[counter][m];
					}
				}
			}
			++counter;
		}		
	}

	for (int i=0;i<npars;i++)
	{
		// make one-dimensional histograms
		min = TMath::MinElement(lines,param_array_nodups[i]);
		max = TMath::MaxElement(lines,param_array_nodups[i]);
		bins = max - min + 1;
		chi2_hsts[i] = new TH1F(Form("hst%i",i),Form("Parameter %i",pars1d[i]),bins,double(min)-0.5,double(max)+0.5);
		for (int j=0;j<lines;j++)
		{
			bin = chi2_hsts[i]->FindBin(param_array_nodups[i][sorted_indices[j]]);
			chi2_hsts[i]->SetBinContent(bin,chi_array[sorted_indices[j]]);
		}
		chi2_hsts[i]->SetMarkerStyle(20);

		// turn those in to TGraphs
		chi2_1d[i] = new TGraph(chi2_hsts[i]);

		// count how many have more than one point
		if (chi2_1d[i]->GetN()>1)
		{
			left = left + 1;
			params_2d.push_back(i);
		}
	}

	// calculate how many two-dimensional histograms we need
	int sumtemp = 0;
	left = left - 1;
	while (left>0)
	{
		sumtemp = sumtemp + left;
		left = left - 1;
	}

	const int sum = sumtemp;

	cout <<"Need " <<sum <<" two-dimensional chi^2 plots." <<endl;

	if (sum>15) cout <<"Not creating two-dimensional chi^2 plots." <<endl;
	else{
	TGraph2D* chi2_2d[sum];
//	TGraph2D* scales_2d[files][sum];
	TH2F* chi2_2dhsts[sum];
//	TH2F* scales_2dhsts[files][sum];

	// initialize variables for two-dimensional arrays
	int xaxis,yaxis,minx,miny,maxx,maxy,binsx,binsy;
	double minz;
	int num = 0;

	minz = TMath::MinElement(lines,chi_array);

	for (int i=1;i<params_2d.size();i++)
	{
		for (int j=0;j<i;j++)
		{
			// create histograms
			xaxis = params_2d[i];
			yaxis = params_2d[j];
			minx = TMath::MinElement(lines,param_array_nodups[xaxis]);
			maxx = TMath::MaxElement(lines,param_array_nodups[xaxis]);
			miny = TMath::MinElement(lines,param_array_nodups[yaxis]);
			maxy = TMath::MaxElement(lines,param_array_nodups[yaxis]);
			binsx = maxx - minx + 1;
			binsy = maxy - miny + 1;
			chi2_2dhsts[num] = new TH2F(Form("hst%i_2d",num),Form("Parameter %i vs. Parameter %i",pars1d[yaxis],pars1d[xaxis]),binsx,double(minx)-0.5,double(maxx)+0.5,binsy,double(miny)-0.5,double(maxy)+0.5);
			// uncomment next loop to include scales. - JKS
		/*	for (int m=0;m<files;m++)
			{
				scales_2dhsts[m][num] = new TH2F(Form("hst%i_scale%i_2d",num,m),Form("Parameter %i vs. Parameter %i, component %i scale",pars1d[yaxis],pars1d[xaxis],m),binsx,double(minx)-0.5,double(maxx)+0.5,binsy,double(miny)-0.5,double(maxy)+0.5);
			}*/
			for (int k=0;k<lines;k++)
			{
				bin = chi2_2dhsts[num]->FindBin(param_array_nodups[xaxis][sorted_indices[k]],param_array_nodups[yaxis][sorted_indices[k]]);
				chi2_2dhsts[num]->SetBinContent(bin,chi_array[sorted_indices[k]]);
				// uncomment next loop to include scales. - JKS
		/*		for (int m=0;m<files;m++)
				{
					scales_2dhsts[m][num]->SetBinContent(bin,scale_array[m][sorted_indices[k]]);
				}*/
			}

			// test for maxmimum values
			if (maxz<chi2_2dhsts[num]->GetMaximum()) maxz = chi2_2dhsts[num]->GetMaximum();

			// turn those in to TGraphs
			chi2_2d[num] = new TGraph2D(chi2_2dhsts[num]);
			// uncomment next loop to include scales. - JKS
		/*	for (int m=0;m<files;m++)
			{
				scales_2d[m][num] = new TGraph2D(scales_2dhsts[m][num]);	
			}
*/
			// formatting these TGraph2Ds
			chi2_2d[num]->GetXaxis()->SetTitle(Form("Parameter %i",pars1d[xaxis]));
			chi2_2d[num]->GetYaxis()->SetTitle(Form("Parameter %i",pars1d[yaxis]));
			chi2_2d[num]->GetXaxis()->CenterTitle(kTRUE);
			chi2_2d[num]->GetYaxis()->CenterTitle(kTRUE);
			// uncomment next loop to include scales. - JKS
			/*for (int m=0;m<files;m++)
			{
				scales_2d[m][num]->GetXaxis()->SetTitle(Form("Parameter %i",pars1d[xaxis]));
				scales_2d[m][num]->GetYaxis()->SetTitle(Form("Parameter %i",pars1d[yaxis]));
				scales_2d[m][num]->GetXaxis()->CenterTitle(kTRUE);
				scales_2d[m][num]->GetYaxis()->CenterTitle(kTRUE);	
			}*/

			num = num + 1;
		}
	}
	}
/*

	// display all two-dimensional histograms
	TCanvas *c[num];
	for (int i=0;i<num;i++)
	{
		c[i] = new TCanvas();
		chi2_2d[i]->GetZaxis()->SetRangeUser(int(minz-1),int(maxz+1));
		chi2_2d[i]->Draw("colz");
	}*/
}
