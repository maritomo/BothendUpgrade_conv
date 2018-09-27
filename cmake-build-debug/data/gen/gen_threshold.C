{
  int threshold = 200;
  
  std::ofstream ofs("../common_threshold.txt");
  
  for(int slot=0; slot<2; ++slot) {
    for(int ch=0; ch<12; ++ch) {
      ofs << slot << " " << ch << " " << 0 << " " << threshold << "\n";
    }
  }

  ofs.close();
}
