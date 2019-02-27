void gen_runset() {

  std::ofstream ofs("../runset.txt");
  
  int runID[][2] = {
    {1183, 1187}, {1204, 1215}, {1230, 1256}, {1261, 1290},
    {1292, 1319}, {1322, 1351}, {1355, 1382}, {1385, 1432},
    {1435, 1456}, {1458, 1468}, {1470, 1497}, {1499, 1509},
    {1511, 1520}, {1521, 1528}, {1531, 1558}
  };

  int veto[][2] = {{1393, 1393}, {1403, 1425}, {1550, 1550}};
  
  for(int i=0; i<sizeof(runID)/sizeof(int)/2; ++i) {
    int id = runID[i][0];
    while(id <= runID[i][1]) {
      int flag = false;

      for(int j=0; j<sizeof(veto)/sizeof(int)/2; ++j) {      
	int vt = veto[j][0];
	while(vt <= veto[j][1]) {
	  if(id==vt) {
	    flag = true;
	    break;
	  }
	  ++vt;
	}
	if(flag) break;
      }
      
      if(!flag) ofs << id << " ";
      ++id;
    }
    ofs << "\n";
  }

  ofs.close();
}
