void temp(){

  Double_t locationx[50][50];
  Double_t locationy[50][50];
  Double_t locationz[50][50];

  Double_t locationox[50][50];
  Double_t locationoy[50][50];
  Double_t locationoz[50][50];

  TString letter[19] = {"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S"};
  Int_t lll = 0;

  for (int l=0;l<9;++l)
    {
      for (int b=0;b<8;++b)
	{
	  locationx[l][b] = -48.5678 - 5.13*cos(86.395*(TMath::Pi()/180.)) - 10.26*l*cos(86.395*(TMath::Pi()/180.));
	  locationy[l][b] = 10.26*b - 36.04;
	  locationz[l][b] = 639.9243 + 5.13*sin(86.395*(TMath::Pi()/180.)) + 10.26*l*sin(86.395*(TMath::Pi()/180.));
	  locationox[l][b] = cos(86.395*TMath::Pi()/180.);
	  locationoy[l][b] = 0.;
	  locationoz[l][b] = sin(86.395*TMath::Pi()/180.);

	  if(b==0){
	    
	    printf("%f 0.0 %f 8 %f 0.0 %f 00.0 MoNABar %s\n",locationx[l][b],locationz[l][b],locationox[l][b],locationoz[l][b],letter[lll].Data(););
	    lll++;
	  }


	}
    }

  // MoNA' position varibales
  for (int l=0;l<4;++l)
    {
      for (int b=8;b<16;++b)
	{
	  locationx[l][b] = 184.4088 + 5.13*cos(76.8605*(TMath::Pi()/180.)) + 10.26*l*cos(76.8605*(TMath::Pi()/180.));
	  locationy[l][b] = 10.26*(b-8) - 36.04;
	  locationz[l][b] = 426.8335 + 5.13*sin(76.8605*(TMath::Pi()/180.)) + 10.26*l*sin(76.6805*(TMath::Pi()/180.));
	  locationox[l][b] = -cos(76.6805*(TMath::Pi()/180.));
	  locationoy[l][b] = 0.;
	  locationoz[l][b] = sin(76.8605*(TMath::Pi()/180.));

	  if(b==8){
	    
	    printf("%f 0.0 %f 8 %f 0.0 %f 00.0 MoNABar %s\n",locationx[l][b],locationz[l][b],locationox[l][b],locationoz[l][b],letter[lll].Data(););
	    lll++;	  }

	}
    }
  for (int l=4;l<9;++l)
    {
      for (int b=8;b<16;++b)
	{
	  locationx[l][b] = 207.0197 + 5.13*cos(77.1846*(TMath::Pi()/180.)) + 10.26*(l-4)*cos(77.1846*(TMath::Pi()/180.));
	  locationy[l][b] = 10.26*(b-8) - 36.04;
	  locationz[l][b] = 523.6891 + 5.13*sin(77.1846*(TMath::Pi()/180.)) + 10.26*(l-4)*sin(77.1846*(TMath::Pi()/180.));
	  locationox[l][b] = -cos(77.1846*(TMath::Pi()/180.));
	  locationoy[l][b] = 0.;
	  locationoz[l][b] = sin(77.1846*(TMath::Pi()/180.));

	  if(b==8){
	    
	    printf("%f 0.0 %f 8 %f 0.0 %f 00.0 MoNABar %s\n",locationx[l][b],locationz[l][b],locationox[l][b],locationoz[l][b],letter[lll].Data(););
	    lll++;	  }

	}
    }

}
