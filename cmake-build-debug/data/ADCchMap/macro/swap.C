{
  std::string filename = "../run1204_mppc.txt";
  std::ifstream ifs(filename.c_str());

  filename += "_modified";
  std::ofstream ofs(filename.c_str());

  int crystalID, crate, mod, ch;
  while(ifs >> crystalID >> crate >> mod >> ch) {
    ofs << crystalID << " " << crate << " " << mod << " " << (ch+8)%16 << "\n";
  }

  ifs.close();
  ofs.close();
}
