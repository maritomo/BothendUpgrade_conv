{
  std::ofstream ofs("../map_crc.txt");
  
  const int nCRC = 6;
  const int nLayer = 2;
  const int nCRCtot = nCRC * nLayer;
  
  int i_sct[nCRCtot] = {
    7, 10, 8, 11, 15, 1,
    6, 12, 2,  0, 13, 9
  };

  int direction[nCRCtot] = {
     1,  1,  1,  1, -1,  1,
    -1, -1, -1, -1,  1,  1
  };

  double y_local[2] = {-1360, 1235};
  double z_local[6] = {-225, -130, -40, 40, 130, 225};
  double z_shift[2] = {-9, -5};

  double x[nLayer][nCRC];
  double y[nLayer][nCRC];
  double z[nLayer][nCRC];

  for(int i_layer=0; i_layer<nLayer; ++i_layer) {
    for(int i_z=0; i_z<nCRC; ++i_z) {
      int id = i_layer * nCRC + i_z;

      x[i_layer][i_z] = 0;
      y[i_layer][i_z] = y_local[i_layer];
      z[i_layer][i_z] = z_local[i_z] + z_shift[i_layer];

      ofs << i_sct[id] << "\t"
	  << direction[id] << "\t"
	  << x[i_layer][i_z] << "\t"
	  << y[i_layer][i_z] << "\t"
	  << z[i_layer][i_z] << "\n";
    }
    
  }

  ofs.close();

}
