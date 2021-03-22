#pragma once
#include "TextNode.h"
#include <fstream>
#include <string>
class TextIter;

class Text {
protected:
  TextNode* root;
public:
  Text(char* s = 0);
  ~Text();

  TextIter GetRoot(); //Создание рут
  TextIter Find(char c); //Поиск буквы. Вернет итератор после найденой буквы
  TextIter Find(char* c); //Поиск. Вернет итератор после найденой буквы
  TextIter FindWord(char* c); //Поиск слова. Вернет итератор после найденого слова

  //Доп
  void FileSave(); //Сохранить в файл
  void FileLoad(string tmp); //Загрузить из файла

  void Insert(char* c, TextIter i); //Вставить после i
  void DataInsert(char* c, TextIter i); //Вставить данные после И
  void Delete(int count, TextIter i); //Удалить начиная с i, указанное количество букв
  char* Copy(int count, TextIter i); //Копирует указанное количество букв, начиная с i

  friend std::ostream& operator<<(std::ostream& o, Text& t);
};

class TextIter {
protected:
  Text& text;
  TextNode* node;
  TStack<TextNode*> st;
public:
  TextIter(Text& _text, TextNode* _node, TStack<TextNode*> _st);
  TextIter(const TextIter& t);

  bool GoNext(); //Перейти дальше
  bool GoNextChar(); //Перейти к следующей букве
  bool IsEnd(); //Првоерка конца

  void ResetToString();
  void ResetToWord();

  TextNode* Get(); //Получить весь текст
};


TextIter::TextIter(Text& _text, TextNode* _node, TStack<TextNode*> _st) : text(_text) {
  node = _node;
  st = _st;
}

TextIter::TextIter(const TextIter& t) : text(t.text)
{
  node = t.node;
  st = t.st;
}

bool TextIter::GoNext() //Переход дальше
{
  if (node->GetLevel() == 3)
  {
    if (node->GetNext() != 0) 
    {
      node = node->GetNext();
    }
    else
    {
      node = st.Get();

      if (node->GetNext() != 0) 
      {
        node = node->GetNext();
      }
      else
      {
        node = st.Get();
        if (node->GetNext() != 0) 
        {
          node = node->GetNext();
        }
        else
        {
          node = 0;
          return false;
        }
      }
    }
  }
  else
  {
    st.Push(node);
    node = node->GetDown();
  }
  return true;
}

bool TextIter::GoNextChar()
{
  if (node->GetLevel() == 1) 
  {
    st.Push(node);
    node = node->GetDown();
    st.Push(node);
    node = node->GetDown();
    return true;
  }

  if (node->GetLevel() == 2) 
  {
    st.Push(node);
    node = node->GetDown();
    return true;
  }

  if (node->GetLevel() == 3 && node->GetNext() != nullptr) 
  {
    node = node->GetNext();
    return true;
  }

  if (node->GetLevel() == 3 && node->GetNext() == nullptr) 
  {
    if (st.IsEmpty()) 
    {
      throw -1; //Переполнение стека
    }

    TextNode* temp = st.Get();

    if (temp->GetNext() != nullptr)
    {
      st.Push(temp->GetNext());
      node = temp->GetNext()->GetDown();
      return true;
    }

    return false;
  }
  return false;
}

bool TextIter::IsEnd()
{
  if (node == nullptr) 
  {
    return true;
  }

  if (node->GetLevel() == 1 || node->GetLevel() == 2) 
  {
    return false;
  }

  if (node->GetLevel() == 3 && node->GetNext() != nullptr) 
  {
    return false;
  }

  if (node->GetLevel() == 3 && node->GetNext() == nullptr) 
  {
    if (st.IsEmpty()) 
    {
      throw - 1; //Переполнение стека
    }

    TextNode* prev = st.Get();
    if (prev->GetNext() != nullptr) 
    {
      return false;
    }

    return true;
  }

  return true;
}

void TextIter::ResetToString() {
  while (node->GetLevel() > 1) 
  {
    if (st.IsEmpty())
    {
      throw - 1;
    }

    node = st.Get();
  }
}

void TextIter::ResetToWord()
{
  if (node->GetLevel() == 1)
  {
    st.Push(node);
    node = node->GetDown();
  }
  else if (node->GetLevel() == 3)
  {
    node = st.Get();
  }
}

TextNode* TextIter::Get() 
{
  return node;
}

Text::Text(char* s) 
{
  root = new TextNode(s, 1);
}

Text::~Text()
{
  TextNode::FreeMem();
}

TextIter Text::GetRoot()
{
  TStack<TextNode*> A(100);
  TextIter B(*this, root, A);
  return B;
}

TextIter Text::Find(char c)
{
  TextIter iter = GetRoot();

  while (iter.GoNextChar())
  {
    if (iter.Get()->GetC() == c)
    {
      return iter;
    }
  }
  return iter;
}

TextIter Text::FindWord(char* c) 
{
  TStack<TextNode*> A(100);
  TextIter iter(*this, root, A);

  while (iter.GoNext()) 
  {
    if (iter.Get()->GetLevel() == 2)
    {
      TextIter resuls = iter;
      bool control = true;
      int i = 0;
      int n = strlen(c);

      while (iter.GoNext() && i < n)
      {
        if (iter.Get()->GetLevel() == 2)
        {
          control = false;
          break;
        }

        if (iter.Get()->GetC() != c[i])
        {
          control = false;
          break;
        }
        i++;
      }

      if (control)
      {
        return resuls;
      }
    }
  }
  return iter;
}

TextIter Text::Find(char* s)
{
  TStack<TextNode*> A(100);
  TextIter iter(*this, root, A);
  int n = strlen(s);

  while (iter.GoNextChar()) 
  {
    TextIter cur = iter;
    int i = 0;
    bool control = true;
    bool next = true;
    while (i < n) 
    {
      if (!next)
      {
        control = false;
        break;
      }
      if (s[i] != cur.Get()->GetC()) {
        control = false;
        break;
      }
      next = cur.GoNextChar();
      i++;
    }

    if (control)
    {
      return iter;
    }
  }
}

void Text::FileSave()
{
    ofstream outf("Text.txt");
    if (!outf.is_open())
    {
      throw "Error file";
    }
    else
    {
      outf << *this;
      outf.close();
    }
}

void Text::FileLoad(string tmp) // tmp - путь
{
  string l; //Считываем данные
  
  ifstream inf(tmp);
  ifstream inf_2(tmp);

  if (!inf.is_open())
  {
    throw - 1;
  }
  else
  {
    getline(inf, l); //Считываем
    inf.close(); //Закрываем первый файл

    int size = l.length(); //Записываем длину строки
    char* text = new char[size+1]; //Создаем чар* нужной длинны
    text[size] = '\0'; //Устанавливаем конец строки

    if (!inf_2.is_open()) //Открываем файл второй раз
    {
      throw - 1;
    }
    else
    {
      for (int i = 0; i < size; i++) //Считываем 
      {
        text[i] = inf_2.get();
      }

      inf_2.close(); //Закрываем второй файл
    }
    root = new TextNode(text, 1); //Создаем

    if (text != NULL)
    {
      delete[] text;
    }
  }
}

void Text::Insert(char* c, TextIter i)
{
  TextNode* node = new TextNode(c, i.Get()->GetLevel());
  TextNode* next = i.Get()->GetNext();
  node->SetNext(next);
  i.Get()->SetNext(node);
}

inline void Text::DataInsert(char* k, TextIter i)
{  
  if (strlen(k) == 1) //Если получен символ
  {
    Insert(k, i);
  }
  else //Если символов много
  {
    int level = 2;
    for (int j = 0; j < strlen(k); j++)
    {
      if (k[j] == ' ') //Если есть пробел, то это строка
      {
        level = 1;
        break;
      }
    }

    TextIter glb = i;
    TextNode* next = i.Get()->GetNext();

    if (next->GetC() == ' ' || next->GetC() == '.') //Если данные вставляются после слова
    {
      i.GoNextChar(); //Переходим за пробел или точку
      int size = strlen(k); //Получаем длину полученных данных
      char* z = new char[strlen(k) + 1]; //Создаем новый чар
      z[strlen(k) + 1] = '\0'; //Конец строки

      for (int j = 0; j < strlen(k); j++) //Записываем полученные данные в новый чар
      {
        z[j] = k[j];
      }
      z[strlen(k)] = ' '; //Добавляем пробел

      TextNode* node = new TextNode(z, level); //Создаем ноду с новым чаром

      next->SetNext(node); //Записываем ее
      i.Get()->SetNext(node);
    }
    else //Если данные вставляются в тело слова
    {
      int count = 0;
      char symbol = ' ';
      TextNode* count_node = i.Get()->GetNext(); //Получаем слово

      //Считаем длину остатка слова
      while ((count_node->GetC() != '.') && (count_node->GetC() != ' ') && (count_node->GetC() != '\n'))
      {
        count++;
        count_node = count_node->GetNext();
      }

      if (count_node->GetC() == '.')
      {
        symbol = '.';
      }

      if (count_node->GetC() == '\n')
      {
        symbol = '\n';
      }

      //Создаем итоговые данные для вставки
      char* tmp = new char[strlen(k) + count + 3]; //Создаем новый чар необходимой длины
      tmp[strlen(k) + count + 3] = '\0'; //Конец строки
      tmp[0] = ' '; //Первый символ - пробел

      int j = 1;
      int g = 0;
      //Переносим данные в новый чар
      for (j; j < strlen(k) + 1; j++, g++)
      {
        tmp[j] = k[g];
      }
      tmp[j] = ' '; //Добавляем пробел

      TextNode* tmp_node = i.Get()->GetNext();
      for (j = j + 1; j < strlen(k) + count + 2; j++) //Дописываем остаток слова
      {
        tmp[j] = tmp_node->GetC();
        tmp_node = tmp_node->GetNext();
      }

      tmp[strlen(k) + count + 2] = symbol; //Добавляем символ

      glb.GoNext();
      this->Delete(count, glb);

      TextNode* node = new TextNode(tmp, level); //Создаем ноду с новыми данными
      next->SetNext(node);
      i.Get()->SetNext(node);
    }
  }
}

void Text::Delete(int count, TextIter i)
{
  if (count == 0) //Если удалять нечего - выход
  {
    return;
  }

  TStack<TextNode*> stack[3];
  int symb_del = 0; //Количество удаляемых символов
  int level = i.Get()->GetLevel();

  //Переходим к букве, с которой нужно удалять
  if (level < 3)
  {
    i.GoNextChar();
  }

  if (level >= 1 && level <= 3)
  {
    stack[level - 1].Push(i.Get()); //Добавляем в стек
    if (level == 3)
    {
      symb_del++;
    }
  }

  TextIter iter = i;
  while (iter.GoNext() && symb_del < count)
  {
    level = iter.Get()->GetLevel(); //Получаем уровень

    if (level >= 1 && level <= 3)
    {
      stack[level - 1].Push(iter.Get()); //Добавляем в стек

      if (level == 3)
      {
        symb_del++;
      }
    }
  }

  if (symb_del < count)
    throw - 1;

  //Удаление букв в одном слове
  if (stack[0].IsEmpty() && stack[1].IsEmpty())
  {
    TextIter prev = i;
    prev.ResetToWord(); 
    while (prev.Get()->GetNext() != i.Get() && prev.Get()->GetDown() != i.Get())
    {
      prev.GoNext();
    }
    TextNode* last = stack[2].Get();
    if (prev.Get()->GetLevel() == 2)
    {
      prev.Get()->SetDown(last->GetNext());
    }
    else
    {
      prev.Get()->SetNext(last->GetNext());
    }
    delete last;
    return;
  }

  //Установка nulltr для буквы, после которой начинается удаление
  TextIter prev = i;
  prev.ResetToWord();

  while (prev.Get()->GetNext() != i.Get() && prev.Get()->GetDown() != i.Get())
  {
    prev.GoNext();
  }

  if (prev.Get()->GetNext() == i.Get())
  {
    prev.Get()->SetNext(nullptr);
  }

  else
  {
    prev.Get()->SetDown(nullptr);
  }

  if (stack[0].IsEmpty()) //Удаление букв в одной строке
  {
    TextNode* last_word = stack[1].Get();
    TextNode* last_letter = stack[2].Get();
    TextIter cur = i;
    cur.ResetToWord();
    cur.Get()->SetNext(last_word);
    last_word->SetDown(last_letter->GetNext());
    delete last_letter;
    for (int i = 1; i < 3; i++)
    {
      while (!stack[i].IsEmpty())
      {
        TextNode* del = stack[i].Get();
        delete del;
      }
    }
  }
  else //Удаление букв в разных строках
  {
    TextNode* last_string = stack[0].Get();
    TextNode* last_word = stack[1].Get();
    TextNode* last_letter = stack[2].Get();
    TextIter cur = i;
    cur.ResetToString();
    cur.Get()->SetNext(last_string);
    cur.ResetToWord();
    cur.Get()->SetNext(last_word);
    last_word->SetDown(last_letter->GetNext());
    delete last_letter;

    for (int i = 0; i < 3; i++)
    {
      while (!stack[i].IsEmpty())
      {
        TextNode* tmp = stack[i].Get();
        delete tmp;
      }
    }
  }
}

char* Text::Copy(int count, TextIter i)
{
  if (count < 0)
  {
    throw - 1;
  }
    
  char* result = new char[count + 1];
  result[count] = '\0';
  TextIter iter = i;

  while (iter.Get()->GetLevel() < 3)
  {
    iter.GoNext();
  }

  if (count > 0) 
  {
    result[0] = iter.Get()->GetC();
    int j = 1;
    
    while (iter.GoNextChar() && j < count)
    {
      result[j] = iter.Get()->GetC();
      j++;
    }
  }

  
  return result;
}

std::ostream& operator<<(std::ostream& out, Text& t)
{
  out << *(t.root);
  return out;
}
