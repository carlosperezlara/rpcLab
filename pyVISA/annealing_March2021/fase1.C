#include "helpers.C"

TF1 *gainenf;

void LoadGain() {
  TFile *rfile = new TFile("/Users/cperez/MTDcharacterization/HDR2data/sipm_spec_input_HDR2-015-v2-1e13.root");
  rfile->ls();
  TF1 *gai = (TF1*) rfile->Get("fGain_vs_OV");
  cout << gai->GetFormula()->GetExpFormula() << endl;
  double p0G = gai->GetParameter(0);
  double p1G = gai->GetParameter(1);
  TF1 *enf = (TF1*) rfile->Get("fENF_vs_OV");
  double p0E = enf->GetParameter(0);
  double p1E = enf->GetParameter(1);
  gainenf = new TF1("gainenf",Form("(%f+x*%f)*(1+%f*x+%f*x*x)",p0G,p1G,p0E,p1E),0,10);
}

int fase1() {
  gStyle->SetOptStat(0);
  LoadGain();
  cout << gainenf->GetFormula()->GetExpFormula() << endl;

  TGraph *grREFDa = new TGraph("outputfiles/20200831_HDR2-2e14-2/p20C/dark/HDR2-2e14-2-20C-dark-1.csv","%lg, %lg");
  TGraph *grREFLi = new TGraph("outputfiles/20200831_HDR2-2e14-2/p20C/light/HDR2-2e14-2-20C-11.36mA-Orange-1.csv","%lg, %lg");
  grREFDa->SetMarkerStyle(24);
  grREFDa->SetMarkerColor(kBlue-3);
  grREFLi->SetMarkerStyle(24);
  grREFLi->SetMarkerColor(kRed-3);

  TGraph *grNEWDa = new TGraph("outputfiles/fase1/outputdata_DARK.csv","%*s %*s %lg, %*s %*s %lg, %*s %*s");
  TGraph *grNEWLi = new TGraph("outputfiles/fase1/outputdata_LED.csv","%*s %*s %lg, %*s %*s %lg, %*s %*s");
  grNEWDa->SetMarkerStyle(20);
  grNEWDa->SetMarkerColor(kBlue-3);
  grNEWLi->SetMarkerStyle(20);
  grNEWLi->SetMarkerColor(kRed-3);

  
  TCanvas *main = new TCanvas();
  TH2D *axis = new TH2D("axis",";bias  (V);current  (A)",100,0,43,1000,1e-8,1e-2);
  main->SetLogy(1);
  axis->Draw();
  grREFDa->Draw("PLSAME");
  grREFLi->Draw("PLSAME");
  grNEWDa->Draw("PLSAME");
  grNEWLi->Draw("PLSAME");

  TF1 *fit[5];
  for(int i=0; i!=5; ++i) {
    fit[i] = new TF1( Form("fit%d",i), "expo", -5, +3 );
  }
  TCanvas *main2 = new TCanvas();
  TH2D *axispreDCR = new TH2D("axispreDCR",";bias  -  vbr  (V);current  (A)",100,-18,2,1000,1e-8,1e-2);
  main2->SetLogy(1);
  axispreDCR->Draw();
  TGraph *grREFfin = new TGraph("outputfiles/20200831_HDR2-2e14-2/p20C/dark/HDR2-2e14-2-20C-dark-1.csv","%lg, %lg");
  TGraph *grNEWfin = new TGraph("outputfiles/fase1/outputdata_DARK.csv","%*s %*s %lg, %*s %*s %lg, %*s %*s");
  TGraph *grNEWfin_1 = new TGraph("outputfiles/fase1/outputdata_DARK_1.csv","%*s %*s %lg, %*s %*s %lg, %*s %*s");
  TGraph *grNEWfinA = new TGraph("outputfiles/fase1/outputdata_DARK_2_AFTER.csv","%*s %*s %lg, %*s %*s %lg, %*s %*s");
  TGraph *grNEWfinA_1 = new TGraph("outputfiles/fase1/outputdata_DARK_1_AFTER.csv","%*s %*s %lg, %*s %*s %lg, %*s %*s");
  for(int i=0; i!=grNEWfin->GetN(); ++i) { //shift X
    grREFfin->SetPointX( i, grREFfin->GetPointX(i)-41.0126 );
    grNEWfin->SetPointX( i, grNEWfin->GetPointX(i)-41.3856 );
    grNEWfin_1->SetPointX( i, grNEWfin_1->GetPointX(i)-41.2414 );
    grNEWfinA->SetPointX( i, grNEWfinA->GetPointX(i)-41.3856 );
    grNEWfinA_1->SetPointX( i, grNEWfinA_1->GetPointX(i)-41.2414 );
  }
  grREFfin->Fit( fit[0], "R", "", -21, -6 );//-14, -4 );
  grNEWfin->Fit( fit[1], "R", "", -21, -6 );
  grNEWfin_1->Fit( fit[2], "R", "", -21, -6 );
  grNEWfinA->Fit( fit[3], "R", "", -21, -6 );
  grNEWfinA_1->Fit( fit[4], "R", "", -21, -6 );
  grREFfin->SetMarkerStyle(20);
  grREFfin->SetMarkerColor(kBlue-3);
  grNEWfin->SetMarkerStyle(20);
  grNEWfin->SetMarkerColor(kRed-3);
  grNEWfin_1->SetMarkerStyle(20);
  grNEWfin_1->SetMarkerColor(kOrange-3);
  grNEWfinA->SetMarkerStyle(24);
  grNEWfinA->SetMarkerColor(kRed-3);
  grNEWfinA_1->SetMarkerStyle(24);
  grNEWfinA_1->SetMarkerColor(kOrange-3);

  grREFfin->Draw("*SAME");
  grNEWfin->Draw("PSAME");
  grNEWfin_1->Draw("PSAME");
  grNEWfinA->Draw("PSAME");
  grNEWfinA_1->Draw("PSAME");

  TLegend *leg = new TLegend(0.1,0.72,0.5,0.9);
  leg->AddEntry( grREFfin, "[2] Oct 15th 2020 @ ~20C" );
  leg->AddEntry( grNEWfin, "[2] Before fase1 RoomTemp" );
  leg->AddEntry( grNEWfin_1, "[1] Before fase1 RoomTemp" );
  leg->AddEntry( grNEWfinA, "[2] After fase1 RoomTemp" );
  leg->AddEntry( grNEWfinA_1, "[1] After fase1 RoomTemp" );
  leg->Draw();


  TCanvas *main3 = new TCanvas();
  TH2D *axisDCR = new TH2D("axisDCR",";bias  -  vbr  (V);current  -  leak  (A)",100,-5,2,1000,1e-8,1e-2);
  main3->SetLogy(1);
  axisDCR->Draw();
  TGraph *grREFfinX = new TGraph("outputfiles/20200831_HDR2-2e14-2/p20C/dark/HDR2-2e14-2-20C-dark-1.csv","%lg, %lg");
  TGraph *grNEWfinX = new TGraph("outputfiles/fase1/outputdata_DARK.csv","%*s %*s %lg, %*s %*s %lg, %*s %*s");
  TGraph *grNEWfinX_1 = new TGraph("outputfiles/fase1/outputdata_DARK_1.csv","%*s %*s %lg, %*s %*s %lg, %*s %*s");
  TGraph *grNEWfinXA = new TGraph("outputfiles/fase1/outputdata_DARK_2_AFTER.csv","%*s %*s %lg, %*s %*s %lg, %*s %*s");
  TGraph *grNEWfinXA_1 = new TGraph("outputfiles/fase1/outputdata_DARK_1_AFTER.csv","%*s %*s %lg, %*s %*s %lg, %*s %*s");
  for(int i=0; i!=grNEWfinX->GetN(); ++i) { //shift X
    grREFfinX->SetPointX( i,   grREFfinX->GetPointX(i)-41.0126 );
    grNEWfinX->SetPointX( i,   grNEWfinX->GetPointX(i)-41.3856 );
    grNEWfinX_1->SetPointX( i, grNEWfinX_1->GetPointX(i)-41.2414 );
    grNEWfinXA->SetPointX( i,   grNEWfinXA->GetPointX(i)-41.3667 );
    grNEWfinXA_1->SetPointX( i, grNEWfinXA_1->GetPointX(i)-41.4691 );
    grREFfinX->SetPointY( i,   grREFfinX->GetPointY(i)-fit[0]->Eval( grREFfinX->GetPointX(i) ) );
    grNEWfinX->SetPointY( i,   grNEWfinX->GetPointY(i)-fit[1]->Eval( grNEWfinX->GetPointX(i) ) );
    grNEWfinX_1->SetPointY( i, grNEWfinX_1->GetPointY(i)-fit[2]->Eval( grNEWfinX_1->GetPointX(i) ) );
    grNEWfinXA->SetPointY( i,   grNEWfinXA->GetPointY(i)-fit[3]->Eval( grNEWfinXA->GetPointX(i) ) );
    grNEWfinXA_1->SetPointY( i, grNEWfinXA_1->GetPointY(i)-fit[4]->Eval( grNEWfinXA_1->GetPointX(i) ) );
    //cout << fit[0]->Eval( grREFfinX->GetPointX(i) ) << endl;
  }
  grREFfinX->SetMarkerStyle(20);
  grREFfinX->SetMarkerColor(kBlue-3);
  grNEWfinX->SetMarkerStyle(20);
  grNEWfinX->SetMarkerColor(kRed-3);
  grNEWfinX_1->SetMarkerStyle(20);
  grNEWfinX_1->SetMarkerColor(kOrange-3);
  grNEWfinXA->SetMarkerStyle(24);
  grNEWfinXA->SetMarkerColor(kRed-3);
  grNEWfinXA_1->SetMarkerStyle(24);
  grNEWfinXA_1->SetMarkerColor(kOrange-3);

  grREFfinX->Draw("*SAME");
  grNEWfinX->Draw("PSAME");
  grNEWfinX_1->Draw("PSAME");
  grNEWfinXA->Draw("PSAME");
  grNEWfinXA_1->Draw("PSAME");

  leg->Draw();


  TCanvas *main4 = new TCanvas();
  TH2D *axisDCR2 = new TH2D("axisDCR2",";bias  -  vbr  (V);DCR  (GHz)",100,0,0.5,1000,1,1e3);
  main4->SetLogy(1);
  axisDCR2->Draw();
  TGraph *grREFfinXX = new TGraph("outputfiles/20200831_HDR2-2e14-2/p20C/dark/HDR2-2e14-2-20C-dark-1.csv","%lg, %lg");
  TGraph *grNEWfinXX = new TGraph("outputfiles/fase1/outputdata_DARK.csv","%*s %*s %lg, %*s %*s %lg, %*s %*s");
  TGraph *grNEWfinXX_1 = new TGraph("outputfiles/fase1/outputdata_DARK_1.csv","%*s %*s %lg, %*s %*s %lg, %*s %*s");
  TGraph *grNEWfinXXA = new TGraph("outputfiles/fase1/outputdata_DARK_2_AFTER.csv","%*s %*s %lg, %*s %*s %lg, %*s %*s");
  TGraph *grNEWfinXXA_1 = new TGraph("outputfiles/fase1/outputdata_DARK_1_AFTER.csv","%*s %*s %lg, %*s %*s %lg, %*s %*s");
  for(int i=0; i!=grNEWfinX->GetN(); ++i) { //shift X
    grREFfinXX->SetPointX( i,   grREFfinXX->GetPointX(i)-41.0126 );
    grNEWfinXX->SetPointX( i,   grNEWfinXX->GetPointX(i)-41.3856 );
    grNEWfinXX_1->SetPointX( i, grNEWfinXX_1->GetPointX(i)-41.2414 );
    grNEWfinXXA->SetPointX( i,   grNEWfinXXA->GetPointX(i)-41.3667 );
    grNEWfinXXA_1->SetPointX( i, grNEWfinXXA_1->GetPointX(i)-41.4691 );
    double ov, factor;
    ov = grREFfinXX->GetPointX(i); factor = 1e-9/(1.6e-19)/gainenf->Eval( ov );
    grREFfinXX->SetPointY( i,      factor*(grREFfinXX->GetPointY(i)-fit[0]->Eval( grREFfinXX->GetPointX(i) )) );

    ov = grNEWfinXX->GetPointX(i); factor = 1e-9/(1.6e-19)/gainenf->Eval( ov );
    grNEWfinXX->SetPointY( i,      factor*(grNEWfinXX->GetPointY(i)-fit[1]->Eval( grNEWfinXX->GetPointX(i) )) );
    ov = grNEWfinXX_1->GetPointX(i); factor = 1e-9/(1.6e-19)/gainenf->Eval( ov );
    grNEWfinXX_1->SetPointY( i,    factor*(grNEWfinXX_1->GetPointY(i)-fit[2]->Eval( grNEWfinXX_1->GetPointX(i) )) );

    ov = grNEWfinXXA->GetPointX(i); factor = 1e-9/(1.6e-19)/gainenf->Eval( ov );
    grNEWfinXXA->SetPointY( i,      factor*(grNEWfinXXA->GetPointY(i)-fit[3]->Eval( grNEWfinXXA->GetPointX(i) )) );
    ov = grNEWfinXXA_1->GetPointX(i); factor = 1e-9/(1.6e-19)/gainenf->Eval( ov );
    grNEWfinXXA_1->SetPointY( i,    factor*(grNEWfinXXA_1->GetPointY(i)-fit[4]->Eval( grNEWfinXXA_1->GetPointX(i) )) );
  }
  grREFfinXX->SetMarkerStyle(20);
  grREFfinXX->SetMarkerColor(kBlue-3);
  grNEWfinXX->SetMarkerStyle(20);
  grNEWfinXX->SetMarkerColor(kRed-3);
  grNEWfinXX_1->SetMarkerStyle(20);
  grNEWfinXX_1->SetMarkerColor(kOrange-3);
  grNEWfinXXA->SetMarkerStyle(24);
  grNEWfinXXA->SetMarkerColor(kRed-3);
  grNEWfinXXA_1->SetMarkerStyle(24);
  grNEWfinXXA_1->SetMarkerColor(kOrange-3);

  grREFfinXX->Draw("*SAME");
  grNEWfinXX->Draw("PSAME");
  grNEWfinXX_1->Draw("PSAME");
  grNEWfinXXA->Draw("PSAME");
  grNEWfinXXA_1->Draw("PSAME");

  leg->Draw();
  

  
  return 0;
}
