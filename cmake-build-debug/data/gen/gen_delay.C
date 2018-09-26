{
  int delay = 14; // must be 0 - 15
  
  std::ofstream ofs("../channel_delay.txt");
  
  for(int slot=0; slot<2; ++slot) {
    for(int ch=0; ch<12; ++ch) {
      ofs << slot << " " << ch << " " << delay << "\n";
    }
  }

  ofs.close();
}
