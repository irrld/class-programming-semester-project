/**
 * @file main.cpp (https://github.com/irrld/class-programming-semester-project)
 * @author Metehan Gezer (irrelevantdev@outlook.com)
 * @brief Algoritmalar ve Programlama Dersi Dönem Ödevi
 * @version 0.1
 * @date 2022-01-19
 *
 * @copyright Copyright (c) 2022
 */
#include <string>
#include <vector>
#include <iostream>
#include <locale>
#include <random>
#include <iterator>
#include <time.h>
#include <map>
#include <algorithm>

 /**
 * Verilen aralıktan rastgele bir kayıt döndürür.
 * Templateleri görmedik fakat Java'da aynı özellik bulunuyor, rastgele video izlerken
 * C++'da da bulunduğunu öğrendim.
 * Burada hangi türde veri gelirse o türde veri döndürmesini sağlıyor.
 * Kaynağım: https://stackoverflow.com/a/13445752
 */
template<typename Iter>
Iter SelectRandomly(Iter start, Iter end) {
	std::advance(start, rand() % std::distance(start, end)); // Iterator'ün başlangıç konumunu rastgele bir sayıda ilerlet
	return start; // İlerletilen iterator'ü geri döndür
}

void ReadFile(const wchar_t* fileName, std::vector<std::wstring>* vector) {
	FILE* f = NULL;
	errno_t error;

	error = _wfopen_s(&f, fileName, L"r, ccs= UTF-8"); // Dosyayı UTF-8 olarak açıyoruz.

	if (error)
	{
		wprintf_s(L"Dosya açılırken bir hata oluştu, hata kodu: %d\n", error);
		return;
	}

	while (!feof(f)) // Dosya sonuna gelene kadar döngüyü devam ettiriyoruz
	{
		wchar_t buf[128]; // Satırları maksimum 128 karakter uzunluğunda okuyacağız
		wchar_t* p = NULL;

		p = fgetws(buf, _countof(buf), f); // Satırı okuyoruz

		if (!p && ferror(f))
		{
			wprintf_s(L"Dosya okunurken bir hata oluştur\n");
			break;
		}
		std::wstring wstring(buf); // wchar_t dizisi ile bir wstring oluşturuyoruz
		wstring.erase(wstring.length() - 1); // Satırın sonundaki \0 (NULL) sembolünü kaldırıyoruz
		vector->push_back(wstring); // Okuduğumuz satırı dosyaya yazıyoruz
	}

	fclose(f); // Dosyayı kapatıyoruz
}


class User {
private:
	std::wstring m_Name, m_Surname;
	int m_Age;
	bool m_QuestionAnswers[10];
	int m_CalculatedPoints;
public:
	User(std::wstring name, std::wstring surname, int age) {
		m_Name = name;
		m_Surname = surname;
		m_Age = age;
		for (int i = 0; i < 10; i++) {
			m_QuestionAnswers[i] = rand() % 2 == 1;
		}
	}

	// Hesaplanmış puanı değiştirir
	void SetCalculatedPoints(int points) {
		m_CalculatedPoints = points;
	}

	// Hesaplanmış puanı geri döndürür
	int GetCalculatedPoints() {
		return m_CalculatedPoints;
	}

	// Verilen numaralı sorunun cevabını döndürür
	bool GetQuestionAnswer(int number) {
		return m_QuestionAnswers[number];
	}

	// Kullanıcının ismini döndürür
	std::wstring GetName() {
		return m_Name;
	}

	// Kullanıcının soy ismini döndürür
	std::wstring GetSurname() {
		return m_Surname;
	}

	// Kullanıcının yaşını döndürür
	int GetAge() {
		return m_Age;
	}

	// Kullanıcının isim ve soyismini döndürür
	std::wstring GetFullName() {
		return m_Name + L" " + m_Surname;
	}
};

struct Question {
	std::string question;
	bool answer;
};

// a ve b eşlerini yaşa göre karşılaştırır
bool CompareByAge(std::pair<std::wstring, User>& a, std::pair<std::wstring, User>& b)
{
	return a.second.GetAge() > b.second.GetAge();
}
// a ve b eşlerini isime göre karşılaştırır
bool CompareByName(std::pair<std::wstring, User>& a, std::pair<std::wstring, User>& b)
{
	return a.second.GetName() < b.second.GetName();
}
// a ve b eşlerini soyisme göre karşılaştırır
bool CompareBySurname(std::pair<std::wstring, User>& a, std::pair<std::wstring, User>& b)
{
	return a.second.GetSurname() < b.second.GetSurname();
}
// a ve b eşlerini puana göre karşılaştırır
bool CompareByPoints(std::pair<std::wstring, User>& a, std::pair<std::wstring, User>& b)
{
	return a.second.GetCalculatedPoints() > b.second.GetCalculatedPoints();
}
// verilen map'i verilen fonksiyonu (comp) kullanarak sıralar ve yazdırır
void SortAndPrint(std::map<std::wstring, User>& map, bool (*comp)(std::pair<std::wstring, User>& a, std::pair<std::wstring, User>& b))
{
	std::vector<std::pair<std::wstring, User> > sortMap;

	for (auto& it : map) {
		sortMap.push_back(it); // Burada map'i vectore çeviriyoruz, böylece std::sort fonksiyonunu kullanabileceğiz
	}

	std::sort(sortMap.begin(), sortMap.end(), comp); // verilen fonksiyonu kullanarak sıralıyoruz

	for (auto& it : sortMap) // auto'yu görmedik fakat kısaca it değişkeninin türünü yazmak yerine o işi derleyiciye bırakıyoruz
	{
		std::wcout << it.first << " (" << it.second.GetAge() << L" yaş) " << it.second.GetCalculatedPoints() << " puan" << std::endl; // yazdırıyoruz
	}
}

/*
Kullandığım sorular, her seferinde tekrar yazmam gerekmesin diye buradan kopyaladım denerken.

Y Barcelona, İspanya'nın başkentidir.
D Alanya, Antalya'nın bir ilçesidir.
Y Türkiye, Avrupa Birliği ülkesidir.
Y İstanbul 14. yüzyılda fethedilmiştir.
D Her doğal sayı tamsayıdır.
D Afrika bir ülke değildir.
Y Her yıl 12 Mayıs Anneler Günü olarak kutlanır.
D Dinozorlarla insanlar aynı anda yeyüzünde bulunmamıştır.
Y Kanada'nın başkenti Toronto'dur.
D Twitter'da karakter sınırı 280'dir.
*/
int main()
{
	// Windows Terminalini Türkçe karakter girişine izin verecek şekilde değiştiriyoruz
	// Eski Windows kurulumumda buna gerek olmuyordu fakat format attıktan sonra bazı
	// ayarlar değişti sanırım, bunu kullanmadan doğru gözükmüyor.
	system("chcp 1254");
	srand((unsigned)time(NULL));
	setlocale(LC_ALL, "TURKISH"); // Dili türkçe yapıyoruz.
	Question* questions = new Question[10]; // Daha sonra cevapları karşılaştırmak için soruları struct dizisinde tutacağız
	// L kullanmamızın sebebi derleyiciye bu string'in unicode olduğunu belirtmek.
	// Ayrıca burada unicode string kullanmazsak dili türkçe olmayan Windows'larda türkçe karakterler bozuluyor.
	// En azından benim kurulumumda bu şekilde oluyor, türkçe bir cihazda denemedim.
	std::wcout << L"Lütfen 10 adet soru giriniz. (Örn. D Dünya güneşin etrafında döner.)\n";

	for (int i = 0; i < 10; i++)
	{
		char answer = getchar(); // Sorunun cevabını kullanıcıdan alıyoruz ve onu answer adlı değişkene atıyoruz.
		// Sorudan önce ve cevaptan sonra olan boşluğa ihtiyacımız yok dolayısıyla boşluğu okuyoruz
		// Aslında buna gerek yok çünkü zaten sorunun içeriğini tekrar kullanıcıya yazdırmıyoruz.
		// Yine de yapmışken tam olsun istedim
		getchar();
		std::string question;
		std::getline(std::cin, question); // Girilen yazının devamını okuyoruz.

		(questions + i)->answer = answer == 'D'; // Girilen D ise doğru, başka bir şey ise yanlış.
		(questions + i)->question = question;
	}

	std::vector<std::wstring> names;
	ReadFile(L"ad.txt", &names); // ad.txt dosyasını oku ve names vectorüne ekle.
	std::vector<std::wstring> surnames;
	ReadFile(L"soyad.txt", &surnames); // soyisim.txt dosyasını oku ve surnames vectorüne ekle.

	// Burada map kullanmamızın sebebi aynı isim soyisim kombinasyonuna sahip başka bir
	// kullanıcı olup olmadığını kontrol edebilmek.
	std::map<std::wstring, User> users;

	int userSize; // Bu kullanıcıları oluşturmak için kullanılacak
	std::wcout << L"Kullanıcı sayısı giriniz.\n";
	std::cin >> userSize;

	for (int i = 0; i < userSize; i++)
	{
		std::wstring name;
		std::wstring surname;
		do
		{
			name = *SelectRandomly(names.begin(), names.end()); // Rastgele isim seç
			surname = *SelectRandomly(surnames.begin(), surnames.end()); // Rastgele soyisim seç
		} while (users.find(name + L" " + surname) != users.end()); // Eğer bu isim soyisim kombinasyonu daha önce alındıysa tekrarlıyoruz
		User user{ name, surname, (rand() % 42) + 18 }; // Seçilen isim soyisim ile bir kullanıcı oluştur ve bu kullanıcıya 18 ile 60 arasında bir yaş belirle.
		int points = 0; // Toplam puan
		for (int i = 0; i < 10; i++)
		{
			Question question = questions[i]; // Başta girilen sorulardan i numaralısını al
			bool answer = user.GetQuestionAnswer(i); // Kullanıcıdan i numaralı soruya verilen cevabı al
			if (question.answer == answer) // Verilen cevap girilen cevap ile aynı ise 10 puan ekle
			{
				points += 10;
			}
		}
		user.SetCalculatedPoints(points);
		users.insert({ user.GetFullName(), user }); // Kullanıcıyı listeye ekle
	}

	while (true)
	{
		std::wcout << L"Kullanıcıları neye göre sıralamak istersin?\nisim, soyisim, yaş, puan (çıkmak için çıkış)\n";
	read:
		std::wstring sort;
		std::wcin >> sort; // Kullanıcıdan sıralama ölçütünü alıyoruz
		// Kullanıcının girdiği ölçüte uygun olanı arıyoruz
		if (sort == L"isim") 
		{
			SortAndPrint(users, CompareByName);
		}
		else if (sort == L"soyisim")
		{
			SortAndPrint(users, CompareBySurname);
		}
		else if (sort == L"yaş")
		{
			SortAndPrint(users, CompareByAge);
		}
		else if (sort == L"puan")
		{
			SortAndPrint(users, CompareByPoints);
		}
		else if (sort == L"çıkış")
		{
			break; // Döngüyü sonlandırır ve programdan çıkar.
		}
		else
		{
			std::wcout << L"Lütfen geçerli bir sıralama ölçütü girin. (çıkmak için çıkış)\n";
			// 'goto' keywordunu görmedik fakat koddaki belirli bir noktaya gitmeyi sağlıyor, kullanmayı pek sevmiyorum normalde
			// ama burada kodu temiz tutmak için kullandım. İlk baştaki yazıyı hatalı girişten sonra tekrar yazmamak için kullanılıyor.
			goto read;
		}
	}
}