#include <gtest.h>
#include <Text.h>

//Возможность создания класса текст, не имеющего данных
TEST(TestText, create_empty_text) {
  ASSERT_NO_THROW(Text T());
}

//Возможность создания класса текст, имеющего данные
TEST(TestText, create_non_empty_text) {
  char* tmp = "New text";
  ASSERT_NO_THROW(Text T(tmp));
}

//Возможность добавления нескольних строк
TEST(TestText, multiple_lines_be_added) {
  char* tmp = "New text. \nNew line of text.";
  ASSERT_NO_THROW(Text T(tmp));
}

//Возможность вставлять данные в текст
TEST(TestText, insert_data_into_text) {
  char* tmp = "New text.";
  Text T(tmp);
  TextIter K = T.Find("x");
  
  ASSERT_NO_THROW(T.DataInsert("Qwerty", K));
}

//Получение первого символа
TEST(TestText, get_the_first_symbol) {
  char* tmp = "New text";
  Text T(tmp);
  TextIter iter = T.GetRoot();
  iter.GoNextChar();
  ASSERT_EQ(iter.Get()->GetC(), 'N');
}

//Получение первого символа следующего слова
TEST(TestText, get_the_first_symbol_of_the_next_word) {
  char* tmp = "New text";
  Text T(tmp);
  TextIter i = T.GetRoot();
  while (i.Get()->GetC() != ' ')
  {
    i.GoNextChar();
  }
  i.GoNextChar();
  ASSERT_EQ(i.Get()->GetC(), 't');
}

//Контроль получения следующего символа(невозможно получить, если там пусто)
TEST(TestText, cannot_get_the_next_symbol_if_there_is_none) {
  char* tmp = "N";
  Text T(tmp);
  TextIter i = T.GetRoot();
  i.GoNextChar();
  ASSERT_EQ(i.GoNextChar(), false);
}

//Контроль получения следующего символа
TEST(TestText, go_next) {
  char* tmp = "New text";
  Text T(tmp);
  TextIter i = T.GetRoot();
  ASSERT_EQ(i.GoNext(), true);
}

//Найти символ, следующий за указанным
TEST(TestText, find_the_symbol) {
  char* tmp = "New text";
  Text T(tmp);
  TextIter i = T.Find('e');
  i.GoNextChar();
  ASSERT_EQ(i.Get()->GetC(), 'w');
}

//вставка указанного символа на место итератора
TEST(TestText, insert_the_symbol) {
  char* tmp = "New text";
  Text T(tmp);
  TextIter i = T.GetRoot();
  ASSERT_NO_THROW(T.Insert("Q", i));
}

//Вывод первой буквы указанного слова
TEST(TestText, find_the_word) {
  char* tmp = "New text";
  Text T(tmp);
  char* tmp_find = "text";
  TextIter i = T.FindWord(tmp_find);
  i.GoNextChar();
  ASSERT_EQ(i.Get()->GetC(), 't');
}

//Удаление по итератору и кол-ву символов
TEST(TestText, text_delete) {
  char* tmp = "New text";
  Text T(tmp);
  char* tmp_find = "New";
  TextIter i = T.FindWord(tmp_find);
  ASSERT_NO_THROW(T.Delete(3, i));
}


//Сохранение в файл
TEST(TestText, save_to_file) {
  char* tmp = "New text";
  Text T(tmp);
  ASSERT_NO_THROW(T.FileSave());
}

//Загрузка из файла
TEST(TestText, load_from_file) {
  char* tmp = "";
  Text T(tmp);
  ASSERT_NO_THROW(T.FileLoad("Text.txt"));
}

//Получение нужного текста в char (в массив)
TEST(TestText, _copy) {
  char* tmp = "New text";
  Text T(tmp);
  char tmp_find = 't';
  TextIter i = T.Find(tmp_find);
  ASSERT_NO_THROW(char* result = T.Copy(4, i));
}