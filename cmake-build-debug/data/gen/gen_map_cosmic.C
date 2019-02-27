{
  std::ofstream ofs("../map_cosmic.txt");
  
  double y_local[2] = {-2163, 2154};
  double z_local[6] = {-225, -130, -40, 40, 130, 225};
  double z_shift[2] = {0, -10};

  double x[2][6];
  double y[2][6];
  double z[2][6];

  for(int i_layer=0; i_layer<2; ++i_layer) {
    for(int i_z=0; i_z<6; ++i_z) {
      int id = 6 * i_layer + i_z;

      x[i_layer][i_z] = 0;
      y[i_layer][i_z] = y_local[i_layer];
      z[i_layer][i_z] = z_local[i_z] + z_shift[i_layer];

      ofs << id << "\t"
	  << 12 * i_layer + 2 * i_z << "\t"
	  << 12 * i_layer + 2 * i_z + 1 << "\t"
	  << x[i_layer][i_z] << "\t"
	  << y[i_layer][i_z] << "\t"
	  << z[i_layer][i_z] << "\n";
    }
  }

  ofs.close();
}
