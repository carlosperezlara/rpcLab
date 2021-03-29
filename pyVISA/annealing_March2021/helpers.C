const int MAX = 10000;
double xT[MAX], yR[MAX];
int nT;

double gainS141601315PS(double x) { // gain as a function of overvoltage valid x = from 3.5 to 6.5 at 25degC
  //https://www.hamamatsu.com/resources/pdf/ssd/s14160-1310ps_etc_kapd1070e.pdf
  return -1899.97 + x*82179.6;
}
  
  
double gainS141601310PS(double x) { // gain as a function of overvoltage valid x = from 3.5 to 7.5 at 25degC
  //https://www.hamamatsu.com/resources/pdf/ssd/s14160-1310ps_etc_kapd1070e.pdf
  return -2029.01 + x*35338.7;
}

int peak(int xstart, int xstop,
         double y[MAX], double &max) {
  int ret = -999;
  max = -999;
  for(int i=xstart; i<xstop; ++i)
    if(y[i]>max) {
      max = y[i];
      ret = i;
    }
  return ret;
}

int floor(int xstart, int xstop,
	  double y[MAX], double &min) {
  double ret = 999;
  min = 999;
  for(int i=xstart; i<xstop; ++i)
    if(y[i]<min) {
      min = y[i];
      ret = i;
    }
  return ret;
}

void loadTemperatureTables() {
  std::ifstream fin( "outputfiles/KT103J2.csv" );
  std::string line;
  nT=0;
  for(;;nT++) {
    if(!std::getline(fin,line)) break;
    if(nT>MAX) break;
    TString rline = line;
    TObjArray *lst = rline.Tokenize(",");
    xT[nT] =  ((TObjString*) lst->At(0))->GetString().Atof();
    yR[nT] =  ((TObjString*) lst->At(1))->GetString().Atof();
    //std::cout << xT[n] << " " << yR[n] << std::endl;
  }
  std::cout << "I found " << nT << " calibration points." << std::endl;
  if(nT<10) {
    exit(0);
  }
}

double mintemp(double temp) {
  for(int i=0; i!=nT; ++i) {
    if(yR[i]<temp) {
      //std::cout << yR[i] << std::endl;
      return xT[i];
    }
  }
  return -999;
}

void readtemperature(TString file,
                     double &center, double &error) {
  vector<double> resist;
  std::ifstream fin( Form("outputfiles/%s",file.Data()) );
  std::string line;
  for(;;) {
    if(!std::getline(fin,line)) break;
    TString rline = line;
    TObjArray *lst = rline.Tokenize(",");
    resist.push_back( ((TObjString*) lst->At(1))->GetString().Atof() );
  }
  std::cout << "I read " << resist.size() << " values of temperature " << std::endl;
  sort( resist.begin(), resist.end() );
  double temp1 = resist[0];
  double temp2 = resist[resist.size()-1];
  std::cout << "First value " << temp1 << " last value " << temp2 << std::endl;
  double min = mintemp( temp1 );
  double max = mintemp( temp2 );
  center = 0.5*(min+max);
  error = 0.5*(max-min);
  std::cout << "Temperature range " << min << ", " << max << std::endl;
}

void readovervoltagefile(TString file,
			 double &voltage, double &current, int n=1
			 ) {
  voltage = -999;
  current = -999;
  ifstream fin( Form("outputfiles/%s_OverVoltage.txt",file.Data()));
  std::cout << Form("outputfiles/%s_OverVoltage.txt",file.Data()) << std::endl;
  TString tmp;
  fin >> tmp;
  if(tmp!=file) {
    std::cout << "Something wrong with this file " << file.Data() << std::endl;
    std::cout << tmp.Data() << std::endl;
    return;
  }
  double volt, curr;
  for(int i=0;;++i) {
    fin >> volt >> curr;
    if(!fin.good()) break;
    std::cout << volt << " " << curr<< std::endl;
    if(i==n) {
      voltage = volt;
      current = curr;
      break;
    }
  }
  fin.close();
}

void readfile(TString file,
              double &landauMean,   double &landauStd,
              double &straightMean, double &straightStd,
              double &linearMean,   double &linearStd
              ) {
  ifstream fin( Form("outputfiles/%s_Fits.txt",file.Data()));
  TString tmp;
  fin >> tmp;
  if(tmp==file) {
    fin >> landauMean   >> landauStd;
    fin >> straightMean >> straightStd;
    fin >> linearMean   >> linearStd;
  } else {
    std::cout << "Something wrong with this file " << file.Data() << std::endl;
    std::cout << tmp.Data() << std::endl;
    exit(0);
  }
}

double readtemperaturetrend(TString file, double temp, int index=3) {
  ifstream fin( Form("outputfiles/%s",file.Data()));
  double p0=0, p1=0;
  for(int i=0; i!=4; ++i) {
    fin >> p0  >> p1;
    if(i==index) break;
  }
  fin.close();
  return p0+p1*temp;
}

void readtemperaturetrendCoef(TString file, double &p0, double &p1, int index=0) {
  ifstream fin( Form("outputfiles/%s",file.Data()));
  for(int i=0; i!=2; ++i) {
    fin >> p0  >> p1;
    if(i==index) break;
  }
  fin.close();
  return;
}

int readfile(TString file, double v[MAX], double c[MAX]) {
  ///////
  // my clumsy way of extracting data from a csv file
  TString outfilestring = Form("outputfiles/%s",file.Data());
  std::ifstream fin( outfilestring.Data() );
  std::string line;
  int n=0;
  for(;;n++) {
    if(!std::getline(fin,line)) break;
    if(n>MAX) break;
    TString rline = line;
    TObjArray *lst = rline.Tokenize(",");
    v[n] =  ((TObjString*) lst->At(0))->GetString().Atof();
    c[n] =  ((TObjString*) lst->At(1))->GetString().Atof();
  }
  std::cout << "From file " << outfilestring.Data() << ". I read " << n << " pairs. My MAX stack was " << MAX << std::endl;
  return n;
}

int readfile_2(TString file, double v[MAX], double c[MAX]) {
  ///////
  // my clumsy way of extracting data from a csv file
  TString outfilestring = Form("outputfiles/%s",file.Data());
  std::ifstream fin( outfilestring.Data() );
  std::string line;
  int n=0;
  for(;;n++) {
    if(!std::getline(fin,line)) break;
    if(n>MAX) break;
    TString rline = line;
    TObjArray *lst = rline.Tokenize(",");
    v[n] =  ((TObjString*) lst->At(1))->GetString().Atof();
    c[n] =  ((TObjString*) lst->At(3))->GetString().Atof();
  }
  std::cout << "From file " << outfilestring.Data() << ". I read " << n << " pairs. My MAX stack was " << MAX << std::endl;
  return n;
}

void subtractreference( int n, double v[MAX], double c[MAX], TSpline *sp, double out[MAX] ) {
  for( int i=0; i!=n; ++i )
    out[i] = c[i] - sp->Eval( v[i] );
}

void subtractreference_2( int n, double v[MAX], double c[MAX], double cref[MAX], double out[MAX] ) {
  for( int i=0; i!=n; ++i )
    out[i] = c[i] - cref[i];
}
