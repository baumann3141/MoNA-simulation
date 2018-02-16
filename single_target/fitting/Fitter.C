#include<Looper.C>
#include<fstream>
#include"TString.h"
#include<fit2dHist.C>

vector<double> results[components+1];
ofstream out; // output file for chi^2 data
vector< vector<int> > tofit;

bool find_indices(Looper loops, vector<double> pars[components][maxparam], vector<int> tests[3])
{
  out.open(Form("output/10li_%icomponents_indices.dat",components),std::ofstream::out);
  
  vector<int> testcase;
  bool fit;
  
  while (!loops.Finished())
    {
      testcase = loops.GetNext();
      fit=true;
      // check to make sure parameters match
      for (int i=0;i<tests[0].size();i++)
	{
	  for (int j=0;j<2;j++)
	    {
	      if (tests[2][i]==1)
		{
		  if (pars[tests[0][i]][j+2][testcase[tests[0][i]]]!=pars[tests[1][i]][j+2][testcase[tests[1][i]]]) fit=false;
		}
	      else if (tests[2][i]==2)
		{
		  if (pars[tests[0][i]][j][testcase[tests[0][i]]]!=pars[tests[1][i]][j+2][testcase[tests[1][i]]]) fit=false;
		}
	      else if (tests[2][i]==3)
		{
		  if (pars[tests[0][i]][0][testcase[tests[0][i]]]!=pars[tests[1][i]][2][testcase[tests[1][i]]]) fit=false;
		}
	      else
		{
		  if (pars[tests[0][i]][j][testcase[tests[0][i]]]!=pars[tests[1][i]][j][testcase[tests[1][i]]]) fit=false;
		}
	    }
	}
      if (fit)
	{
	  tofit.push_back(testcase);
	}
    }
  cout <<"Total permutations that will be fit: " <<tofit.size() <<endl;
  
  for (int i=0;i<tofit.size();i++)
    {
      for (int j=0;j<components-1;j++)
	{
	  out <<tofit[i][j] <<"\t";
	}
      out <<tofit[i][components-1] <<endl;
    }
  out.close();
}

bool fitting_from_file(TObjArray* hst_array,TObjArray* data_array, vector<double> pars[components][maxparam], int param_num[components], vector< vector <int> > indices,int part,int parts)
{
  out.open(Form("output/10li_%icomponents_chi2_part%i.dat",components,part),std::ofstream::out);
  
  double chi2;
  
  cout <<"Total permutations in array: " <<indices.size() <<endl;
  cout <<"Total number of parts: " <<parts <<endl;
  int begin,end;
  begin = (indices.size()/parts)*part;
  end = (indices.size()/parts)*(part+1);
  if (parts==part+1)
    {
      end = indices.size();
    }
  cout <<"Fitting permutations " <<begin <<" to " <<end-1 <<endl;
  
  AssignExpHists(data_array);
  for (int i=begin;i<end;i++)
    {
      //if it fits, then fit it!
      AssignSimHists(hst_array,indices[i]);
      chi2=fit2dHist_Run(11);
      
      out <<chi2 <<"\t";
      for (int j=0;j<components;j++)
	{
	  for (int k=0;k<param_num[j];k++)
	    {
	      out <<pars[j][k][indices[i][j]] <<"\t";
	    }
	}
      if (asymErrors) for (int j=0;j<components;j++) out <<fScale[j] <<"\t" <<fErrors[j][0] <<"\t" <<fErrors[j][1] <<"\t";
      else for (int j=0;j<components;j++) out <<fScale[j] <<"\t";
      out <<endl;
    }
  out.close();
}
bool fit_n_params(TObjArray* hst_array,TObjArray* data_array, Looper loops, vector<double> pars[components][maxparam], int param_num[components], vector<int> tests[3])
{
  out.open(Form("output/10li_%icomponents_chi2.dat",components),std::ofstream::out);
  
  vector<int> testcase;
  bool fit;
  double chi2;
  
  while (!loops.Finished())
    {
      testcase = loops.GetNext();
      fit=true;
      // check to make sure parameters match
      for (int i=0;i<tests[0].size();i++)
	{
	  for (int j=0;j<2;j++)
	    {
	      if (tests[2][i]==1)
		{
		  if (pars[tests[0][i]][j+2][testcase[tests[0][i]]]!=pars[tests[1][i]][j+2][testcase[tests[1][i]]]) fit=false;
		}
	      else if (tests[2][i]==2)
		{
		  if (pars[tests[0][i]][j][testcase[tests[0][i]]]!=pars[tests[1][i]][j+2][testcase[tests[1][i]]]) fit=false;
		}
	      else if (tests[2][i]==3)
		{
		  if (pars[tests[0][i]][0][testcase[tests[0][i]]]!=pars[tests[1][i]][2][testcase[tests[1][i]]]) fit=false;
		}
	      else
		{
		  if (pars[tests[0][i]][j][testcase[tests[0][i]]]!=pars[tests[1][i]][j][testcase[tests[1][i]]]) fit=false;
		}
	    }
	}
      if (fit)
	{
	  tofit.push_back(testcase);
	}
    }
  cout <<"Total permutations that will be fit: " <<tofit.size() <<endl;
  
  AssignExpHists(data_array);
  for (int i=0;i<tofit.size();i++)
    {
      //if it fits, then fit it!
      AssignSimHists(hst_array,tofit[i]);
      chi2=fit2dHist_Run(11);
      
      out <<chi2 <<"\t";
      for (int j=0;j<components;j++)
	{
	  for (int k=0;k<param_num[j];k++)
	    {
	      out <<pars[j][k][tofit[i][j]] <<"\t";
	    }
	}
      if (asymErrors)	for (int j=0;j<components;j++) out <<fScale[j] <<"\t";
      else for (int j=0;j<components;j++) out <<fScale[j] <<"\t" <<fErrors[j][0] <<"\t" <<fErrors[j][1] <<"\t";
      out <<endl;
    }
  out.close();
}

vector<double> removeDuplicates(vector<double> vec)
{
  sort(vec.begin(),vec.end());
  vec.erase(unique(vec.begin(),vec.end()),vec.end());
  return vec;
}

vector<double> vecOR(vector<double> vec1, vector<double> vec2)
{
  vector<double> temp = vec1;
  temp.insert(temp.end(),vec2.begin(),vec2.end());
  vector<double> final = removeDuplicates(vec1);
  //	sort(temp.begin(),temp.end());
  //temp.erase(unique(temp.begin(),temp.end()),temp.end());
  return temp;
}

vector<double> vecAND(vector<double> vec1, vector<double> vec2)
{
  vector<double> temp;
  for (int i=0;i<vec1.size();i++)
    {
      for (int j=0;j<vec2.size();j++)
	{
	  if (vec1[i]==vec2[j])
	    {
	      temp.push_back(vec1[i]);
	      break;
	    }
	}
    }
  return temp;
}

vector<int> vecAND(vector<int> vec1, vector<int> vec2)
{
  vector<int> temp;
  for (int i=0;i<vec1.size();i++)
    {
      for (int j=0;j<vec2.size();j++)
	{
	  if (vec1[i]==vec2[j])
	    {
	      temp.push_back(vec1[i]);
	      break;
	    }
	}
    }
  return temp;
}
