// ckde.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <math.h>
#include <vector>
#include <string>
#include <numeric>
#include <functional>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <fstream>

const double e = 2.7182818284;
const double gaussian_coeff = 0.3989422804;

using namespace std;

double kernal_funct(string kernal, double u)
{
	if (kernal == "epanechnikov")
	{
		if (abs(u) <= 1)
		{
			return ((3 / 4)*(1 - (pow(u, 2))));
		}
		else
		{
			return 0;
		}
	}
	else if (kernal == "uniform")
	{
		if (abs(u) <= 1)
		{
			return (1 / 2);
		}
		else
		{
			return 0;
		}
	}
	else if (kernal == "gaussian")
	{
		return (gaussian_coeff * (1 / pow(e, .5*pow(abs(u), 2))));
	}
	else if (kernal == "triangular")
	{
		if (abs(u) <= 1)
		{
			return 1 - abs(u);
		}
		else
		{
			return 0;
		}
	}
	return 0;
}

double stdev(vector<double> &data)
{
	double sum = accumulate(data.begin(), data.end(), 0.0);
	double mean = sum / data.size();

	vector<double> diff(data.size());
	transform(data.begin(), data.end(), diff.begin(),
		bind2nd(std::minus<double>(), mean));
	double sq_sum = inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
	return sqrt(sq_sum / (data.size() - 1));
}

void fromdata(vector<double>* idata, vector<double>* plotpointsx, 
				vector<double>* plotpointsy, string bandwidth="*1", 
				string kernal="gaussian", int resolution=1000)
{
	vector<double> data = *idata;
	double h = 1.06*stdev(data)*exp(pow((data).size(),(-1 / 5)));
	if (bandwidth.substr(0,1) == "*")
	{
		double dband = strtod(bandwidth.substr(1, bandwidth.size()-1).c_str(), NULL);
		if (dband != 0)
		{
			h *= dband;
		}
	}
	else
	{
		double dband = strtod(bandwidth.c_str(), NULL);
		if (dband != 0)
		{
			h = dband;
		}
	}
	
	double plotstart = data[0];
	double plotstop = data[0];
	for each (double val in data)
	{
		if (val < plotstart)
		{
			plotstart = val;
		}
		if (val > plotstop) 
		{
			plotstop = val;
		}
	}
	plotstart -= h;
	plotstop += h;

	double pointsep = (plotstop - plotstart) / resolution;

	double x = plotstart;
	while (x <= plotstop)
	{
		(*plotpointsx).push_back(x);
		double kernalsum = 0;
		double d = 0;
		for each (double val in data)
		{
			//cout << val << endl;
			double u = (x - val) / h;
			//cout << u << endl;
			kernalsum += kernal_funct(kernal, u);
		}
		(*plotpointsy).push_back((1 / (data.size()*h)) * kernalsum);
		x += pointsep;
	}
}

void readdata(string filename, vector<double>* output)
{
	ifstream infile(filename);
	string val;
	while (getline(infile, val))
	{
		(*output).push_back(strtod(val.c_str(), NULL));
	}
	infile.close();
}

void writedata(string filename, vector<double>* ixvals, vector<double>* iyvals)
{
	vector<double> xvals = *ixvals;
	vector<double> yvals = *iyvals;
	ofstream outfile;
	outfile.open(filename);
	if (outfile.is_open())
	{
		for (size_t i = 0; i < xvals.size(); i++)
		{
			outfile << xvals[i] << ',' << yvals[i] << endl;
		}
		outfile.close();
	}
	else cout << "Unable to open file";
}

int main(int argc, char* argv[])
{
	vector<double> data;
	readdata(argv[1], &data);
	vector<double> xpoints;
	vector<double> ypoints;
	bool analyzed = false;
	if (argc == 2)
	{
		fromdata(&data, &xpoints, &ypoints);
		analyzed = true;
	}
	else if (argc == 3)
	{
		fromdata(&data, &xpoints, &ypoints, argv[3]);
		analyzed = true;
	}
	else if (argc == 4)
	{
		fromdata(&data, &xpoints, &ypoints, argv[3], argv[4]);
		analyzed = true;
	}
	else if (argc >= 5)
	{
		fromdata(&data, &xpoints, &ypoints, argv[3], argv[4], strtod(argv[5], NULL));
		analyzed = true;
	}

	if (analyzed)
	{
		writedata(argv[2], &xpoints, &ypoints);
	}
	return 0;
}