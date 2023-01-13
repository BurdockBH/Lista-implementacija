#include <iostream>
#include <stdexcept>

template <typename Tip> class Lista {
public:
  virtual ~Lista(){};
  virtual int brojElemenata() const = 0;
  virtual Tip trenutni() const = 0;
  virtual Tip &trenutni() = 0;
  virtual bool prethodni() = 0;
  virtual bool sljedeci() = 0;
  virtual void pocetak() = 0;
  virtual void kraj() = 0;
  virtual void obrisi() = 0;
  virtual void dodajIspred(const Tip &el){};
  virtual void dodajIza(const Tip &el){};
  virtual Tip &operator[](int indeks) const = 0;
};

template <typename Tip> struct Cvor {
  Tip element;
  Cvor *sledeci;
  Cvor *prethodni;
};

template <typename Tip> class DvostrukaLista : public Lista<Tip> {
  int brElemenata;
  Cvor<Tip> *pocet;
  Cvor<Tip> *kr;
  Cvor<Tip> *tren;

public:
  DvostrukaLista()
      : brElemenata(0), tren(nullptr), pocet(nullptr), kr(nullptr) {}
  ~DvostrukaLista();
  void pocetak() override {
    if (brElemenata == 0)
      throw std::domain_error("Prazna lista");
    tren = pocet;
  }
  void kraj() override {
    if (brElemenata == 0)
      throw std::domain_error("Prazna lista");
    tren = kr;
  }
  int brojElemenata() const override { return brElemenata; }
  Tip trenutni() const override {
    if (brElemenata == 0)
      throw std::domain_error("Prazna lista!");
    return tren->element;
  }
  Tip &trenutni() override {
    if (brElemenata == 0)
      throw std::domain_error("Prazna lista!");
    return tren->element;
  }
  void dodajIspred(const Tip &el) override;
  void dodajIza(const Tip &el) override;
  bool sljedeci() override;
  bool prethodni() override;
  void obrisi() override;
  Tip &operator[](int indeks) const override;
  DvostrukaLista(DvostrukaLista &&l);
  DvostrukaLista(const DvostrukaLista &l);
  DvostrukaLista &operator=(DvostrukaLista &&l);
  DvostrukaLista &operator=(const DvostrukaLista &l);
};

template <typename Tip>
DvostrukaLista<Tip>::DvostrukaLista(const DvostrukaLista &l) {
  brElemenata = l.brElemenata;
  Cvor<Tip> *prati;
  for (Cvor<Tip> *it = l.pocet; it != nullptr; it = it->sledeci) {
    Cvor<Tip> *kopija = new Cvor<Tip>{it->element, nullptr};

    if (it == l.pocet) {
      pocet = kopija;
      prati = kopija;
    } else {
      prati->sledeci = kopija;
      prati = kopija;
    }

    if (it == l.kr)
      kr = kopija;
    if (it == l.tren)
      tren = kopija;
  }
}

template <typename Tip>
DvostrukaLista<Tip>::DvostrukaLista(DvostrukaLista &&l) {
  std::swap(pocet, l.pocet);
  std::swap(kr, l.kr);
  std::swap(tren, l.tren);
  brElemenata = l.brElemenata;
}

template <typename Tip>
DvostrukaLista<Tip> &
DvostrukaLista<Tip>::operator=(const DvostrukaLista<Tip> &l) {
  if (&l != this) {

    if (brElemenata != 0) {
      Cvor<Tip> *brisi = pocet;
      for (Cvor<Tip> *it = pocet; it != nullptr; it = it->sledeci) {
        if (it != pocet) {
          delete brisi;
          brisi = it;
        }
      }
      delete brisi;
    }

    brElemenata = l.brElemenata;
    Cvor<Tip> *prati;
    for (Cvor<Tip> *it = l.pocet; it != nullptr; it = it->sledeci) {
      Cvor<Tip> *kopija = new Cvor<Tip>{it->element, nullptr};

      if (it == l.pocet) {
        pocet = kopija;
        prati = kopija;
      } else {
        prati->sledeci = kopija;
        prati = kopija;
      }

      if (it == l.kr)
        kr = kopija;
      if (it == l.tren)
        tren = kopija;
    }
  }

  return *this;
}

template <typename Tip>
DvostrukaLista<Tip> &DvostrukaLista<Tip>::operator=(DvostrukaLista<Tip> &&l) {
  std::swap(pocet, l.pocet);
  std::swap(kr, l.kr);
  std::swap(tren, l.tren);
  brElemenata = l.brElemenata;
  return *this;
}

template <typename Tip> void DvostrukaLista<Tip>::dodajIspred(const Tip &el) {
  Cvor<Tip> *pomocni = new Cvor<Tip>{el, nullptr, nullptr};

  if (brElemenata == 0) {
    pocet = pomocni;
    kr = pomocni;
    tren = pomocni;
  } else {

    if (tren == pocet) {
      pocet = pomocni;
      tren->prethodni = pomocni;
      pomocni->sledeci = tren;
    }

    else {
      Cvor<Tip> *prije = tren->prethodni;
      prije->sledeci = pomocni;
      pomocni->prethodni = prije;
      pomocni->sledeci = tren;
      tren->prethodni = pomocni;
    }
  }

  brElemenata++;
}

template <typename Tip> void DvostrukaLista<Tip>::dodajIza(const Tip &el) {
  Cvor<Tip> *pomocni = new Cvor<Tip>{el, nullptr, nullptr};

  if (brElemenata == 0) {
    pocet = pomocni;
    kr = pomocni;
    tren = pomocni;
  } else if (tren == kr) {
    kr = pomocni;
    tren->sledeci = pomocni;
    pomocni->prethodni = tren;
  } else {
    Cvor<Tip> *iduci = tren->sledeci;
    tren->sledeci = pomocni;
    pomocni->prethodni = tren;
    pomocni->sledeci = iduci;
    iduci->prethodni = pomocni;
  }

  brElemenata++;
}

template <typename Tip> bool DvostrukaLista<Tip>::sljedeci() {
  if (brElemenata == 0)
    throw std::domain_error("Prazna lista");
  if (tren == kr)
    return false;
  tren = tren->sledeci;
  return true;
}

template <typename Tip> bool DvostrukaLista<Tip>::prethodni() {
  if (brElemenata == 0)
    throw std::domain_error("Prazna lista");
  if (tren == pocet)
    return false;
  tren = tren->prethodni;
  return true;
}

template <typename Tip> void DvostrukaLista<Tip>::obrisi() {
  if (brElemenata == 0)
    throw std::domain_error("Prazna lista!");
  Cvor<Tip> *brisi = tren;

  if (brElemenata != 1) {

    if (tren == pocet) {
      tren = tren->sledeci;
      pocet = tren;
      tren->prethodni = nullptr;
    }

    else if (tren == kr) {
      tren = tren->prethodni;
      kr = tren;
      tren->sledeci = nullptr;
    }

    else {
      Cvor<Tip> *prije = tren->prethodni;
      tren = tren->sledeci;

      prije->sledeci = tren;
      tren->prethodni = prije;
    }
  }

  delete brisi;
  brElemenata -= 1;
}

template <typename Tip> Tip &DvostrukaLista<Tip>::operator[](int indeks) const {
  if (brElemenata == 0 || indeks >= brElemenata)
    throw std::domain_error("Prazna lista!");
  auto vrati = pocet;
  int brojac = 0;
  while (brojac != indeks) {
    vrati = vrati->sledeci;
    brojac++;
  }
  return vrati->element;
}

template <typename Tip> DvostrukaLista<Tip>::~DvostrukaLista() {
  if (brElemenata != 0) {
    Cvor<Tip> *pomocni = pocet;
    while (pomocni) {
      Cvor<Tip> *obrisani = pomocni;
      pomocni = pomocni->sledeci;
      delete obrisani;
    }
  }
}

template <typename Tip> Tip dajMaksimum(const Lista<Tip> &l) {
  auto pomocna = dynamic_cast<const DvostrukaLista<Tip> &>(l);
  pomocna.pocetak();
  Tip max = pomocna.trenutni();
  for (int i = 0; i < pomocna.brojElemenata(); i++) {
    if (max < pomocna.trenutni())
      max = pomocna.trenutni();
    pomocna.sljedeci();
  }
  return max;
}

DvostrukaLista<int> DvostrukaListaTest(int brElemenata) {
  DvostrukaLista<int> l;
  for (int i = 0; i < brElemenata; i++) {
    l.dodajIspred(i * 2);
  }
  return l;
}

void brElemenataTest(int n) {
  auto l = DvostrukaListaTest(n);
  if (l.brojElemenata() == 5)
    std::cout << "OK\n";
  else
    std::cout << "NIJE OK\n";
}

void treturniTest1(int n) {
  auto l = DvostrukaListaTest(n);
  if (l.trenutni() == 0)
    std::cout << "OK\n";
  else
    std::cout << "NIJE OK\n";
}

void treturniTest2(int n) {
  auto l = DvostrukaListaTest(n);
  l.trenutni() = 1915;
  if (l.trenutni() == 1915)
    std::cout << "OK\n";
  else
    std::cout << "NIJE OK (metoda trenutni - izmjena vrijednosti el)\n";
}

template <typename Tip> void prethodniTest(int n) {
  auto l = DvostrukaListaTest(n);
  l.pocetak();
  if (l.prethodni() == false)
    std::cout << "OK\n";
  else
    std::cout << "NOT OK\n";

  l.sljedeci();
  l.prethodni();
  if (l.trenutni() == l[0])
    std::cout << "OK\n";
  else
    std::cout << "NOT OK\n";
}

template <typename Tip> void sledeciTest() {
  auto l = DvostrukaListaTest(4);
  l.pocetak();
  l.sljedeci();
  if (l.trenutni() == l[1])
    std::cout << "OK \n";
  else
    std::cout << "NOT OK \n";

  l.sljedeci();
  if (l.trenutni() == l[2])
    std::cout << "OK\n";
  else
    std::cout << "NOT OK\n";

  l.sljedeci();
  if (l.sljedeci() == false)
    std::cout << "OK\n";
  else
    std::cout << "NOT OK\n";
}

void pocetakTest(int n) {
  auto l = DvostrukaListaTest(n);
  l.sljedeci();
  l.pocetak();
  if (l.trenutni() == l[0])
    std::cout << "OK\n";
  else
    std::cout << "NOT OK \n";
}

void krajTest() {
  auto n = DvostrukaListaTest(14);
  n.kraj();
  if (n.trenutni() == n[13])
    std::cout << "OK \n";
  else
    std::cout << "NOT OK \n";
}

template <typename Tip> void obrisiTest() {
  DvostrukaLista<Tip> n1;
  try {
    n1.obrisi();
    std::cout << "Greska - trebalo je baciti izuzetak\n";
  } catch (std::domain_error &pourka) {
    std::cout << "OK\n";
  } catch (...) {
    std::cout << "Pogresan tip izuzetka!\n";
  }

  auto n = DvostrukaListaTest(7);
  n.pocetak();
  n.obrisi();
  if (n.trenutni() == n[0])
    std::cout << "OK \n";
  else
    std::cout << "NOT OK \n";

  auto n2 = DvostrukaListaTest(5);
  n2.kraj();
  n2.obrisi();
  if (n2.trenutni() == n[2])
    std::cout << "OK\n";
  else
    std::cout << "NOT OK \n";
}

template <typename Tip> void dodajIspredTest(int n) {

  DvostrukaLista<Tip> l;
  l.dodajIspred(12);
  if (l[0] == 12)
    std::cout << "OK \n";
  else
    std::cout << "NOT OK\n";

  auto l2 = DvostrukaListaTest(n);
  l2.pocetak();
  l2.dodajIspred(111);
  if (l2[0] == 111)
    std::cout << "OK\n";
  else
    std::cout << "NOT OK\n";

  auto l3 = DvostrukaListaTest(n);
  l3.kraj();
  l3.dodajIspred(111);
  if (l3[n - 1] == 111)
    std::cout << "OK\n";
  else
    std::cout << "NOT OK\n";

  auto l4 = DvostrukaListaTest(n);
  l4.sljedeci();
  l4.sljedeci();
  l4.dodajIspred(111);
  if (l4[n - 1] == 111)
    std::cout << "OK\n";
  else
    std::cout << "NOT OK\n";
}

template <typename Tip> void dodajIzaTest(int n) {

  DvostrukaLista<Tip> l;
  l.dodajIza(n);
  if (l[0] == n)
    std::cout << "OK\n";
  else
    std::cout << "NOT OK\n";

  auto l2 = DvostrukaListaTest(n);
  l2.pocetak();
  l2.dodajIza(111);
  if (l2[1] == 111)
    std::cout << "OK\n";
  else
    std::cout << "NOT OK\n";

  auto l3 = DvostrukaListaTest(n);
  l3.kraj();
  l3.dodajIza(111);
  if (l3[n] == 111)
    std::cout << "OK\n";
  else
    std::cout << "NOT OK\n";

  auto l4 = DvostrukaListaTest(n);
  l4.pocetak();
  l4.sljedeci();
  l4.sljedeci();
  l4.dodajIza(111);
  if (l4[3] == 111)
    std::cout << "OK\n";
  else
    std::cout << "NOT OK\n";
}

void operatorTest() {
  auto n = DvostrukaListaTest(3);
  if (n[0] == 2 && n[1] == 4 && n[2] == 0)
    std::cout << "OK\n";
  else
    std::cout << "NOT OK\n";
}

void dajMaksimumTest() {
  auto l = DvostrukaListaTest(10);

  if (dajMaksimum(l) == 18)
    std::cout << "OK\n";
  else
    std::cout << "NOT OK\n";
}

int main() {
  int n = 5;
  brElemenataTest(n);
  treturniTest1(n);
  treturniTest2(n);
  prethodniTest<int>(n);
  sledeciTest<double>();
  pocetakTest(n);
  krajTest();
  obrisiTest<float>();
  dodajIspredTest<double>(n);
  dodajIzaTest<int>(n);
  operatorTest();
  dajMaksimumTest();
  return 0;
}