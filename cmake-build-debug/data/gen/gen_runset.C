void gen_runset() {

  std::ofstream ofs("../runset.txt");
  
  int runID[][2] = {
    {1183, 1187}, {1204, 1215}, {1230, 1256}, {1261, 1290},
    {1292, 1319}
  };
  
  for(int i=0; i<sizeof(runID)/sizeof(int)/2; ++i) {
    int id = runID[i][0];
    while(id <= runID[i][1]) {
      ofs << id << " ";
      ++id;
    }
    ofs << "\n";
  }

  ofs.close();
}
