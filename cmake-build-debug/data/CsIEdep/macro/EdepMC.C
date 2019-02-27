{
  TGraph g(2716);

  ifstream ifs("../mip_MC.txt");

  int id;
  while(ifs >> id) {
    double e;
    ifs >> e;
    g.SetPoint(id, id, e);
  }

  g->Draw("apl");
  g->Fit("pol0", "", "", 0, 2239);
  g->Fit("pol0", "+", "", 2240, 2716);

}
