#include "helpers.C"

//int ivcurve(TString filelight="20200915_HDR2-1/n30C/light/HDR2-0e0-1-n30C-257nA-Orange-1.csv", 
//	    TString fileref="20200915_HDR2-1/n30C/dark/HDR2-0e0-1-n30C-dark-1.csv" ) {
int ivcurve(TString filelight="20200831_HDR2-2e14-2/n30C/light/HDR2-2e14-2-n30C-12mA-Orange-1.csv",
	    TString fileref="20200831_HDR2-2e14-2/n30C/dark/HDR2-2e14-2-n30C-dark-1.csv" ) {

  TString outputfilename = filelight + "_";
  {
    TObjArray *objarray = fileref.Tokenize("/");
    int n = objarray->GetEntries();
    outputfilename += ((TObjString*) objarray->At(n-1))->GetString();
  }
  outputfilename += "_IVCURVE";
  
  gStyle->SetOptStat(0);
  double vlight[MAX];
  double clight[MAX];
  double vref[MAX];
  double cref[MAX];
  double csub[MAX];

  int nlight = readfile( filelight, vlight, clight );
  int nref = readfile( fileref, vref, cref );
  
  // create graphical objects
  TGraph  *grlight  = new TGraph(nlight,vlight,clight);
  TGraph  *grref    = new TGraph(nref,  vref  ,cref  );
  TSpline *spref    = new TSpline3("spref",grref);

  // subtract reference
  subtractreference( nlight, vlight, clight, spref, csub );
  TGraph  *grsub    = new TGraph(nlight,vlight,csub  );
  
  /////////////
  // D O N E //
  /////////////

  
  ///////
  // display all
  double max0, max1;
  double min0, min1;
  peak( 0,nlight,clight,max0);
  floor(0,nlight,clight,min0);
  peak( 0,nref,cref,max1);
  floor(0,nref,cref,min1);
  std::cout << "min0 " << min0 << " max0 " << max0 << std::endl;
  std::cout << "min1 " << min1 << " max1 " << max1 << std::endl;

  TLine *lin = new TLine();
  lin->SetLineColor(kRed-3);
  TLatex *tex = new TLatex();

  TCanvas *main = new TCanvas();
  main->cd()->SetLogy(1);
  TH2D *axis0 = new TH2D("axis0","IV-curve;Bias  (V);I  (A)",100,0, vref[nref-1],10000,1e-12,1);
  axis0->Draw("");
  grlight->Draw("PSAME");
  grref->Draw("PSAME");
  //spref->Draw("SAME");
  grsub->Draw("LSAME");
  
  grsub->SetLineColor(kBlack);
  grref->SetMarkerStyle(20);
  grref->SetMarkerColor(kGreen-3);
  grlight->SetMarkerStyle(20);
  grlight->SetMarkerColor(kRed-3);

  main->SaveAs( Form("outputfiles/%s.pdf",outputfilename.Data()), "pdf" );
  
  ///// Saving fit data
  //std::cout << outputfilename.Data() << std::endl;
  ofstream fout( Form("outputfiles/%s.csv",outputfilename.Data()) );
  for(int i=0; i!=nlight; ++i) {
    fout << Form("%11e, %11e\n", vlight[i], csub[i]);
  }
  
  return 0;
}
