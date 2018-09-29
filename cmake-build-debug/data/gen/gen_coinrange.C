{
  double mean = 31;
  double width = 4;

  double coin_range[2] = {mean-width, mean+width};
  
  std::ofstream ofs("../coincidence_range.txt");
  
  for(int slot=0; slot<2; ++slot) {
    for(int ch=0; ch<12; ++ch) {
      ofs << slot << " " << ch << " " << coin_range[0] << " " << coin_range[1] << "\n";
    }
  }

  ofs.close();
}
