#pragma once
#include <iostream>
#include "Stack.h"
#include <cstring>

class TextNode
{
protected:
  static TextNode* firstFree; //Список доступных элементов
  static TextNode* textNodes; //Данные
  static TextNode* textNodes_tmp; //Для перепаковки памяти

  TextNode* next; //Указатель на соседний элемент
  TextNode* down; //Указатель на элемент, который находится глубже
  char c; //Буква
  int level; //Уровень. 0 - текст, 1 - строка, 2 - слово, 3 - буква

public:
  TextNode(int _l = 3, char _c = 0);
  TextNode(char* _c, int _l); //Принимает слово
  TextNode(const TextNode& _node); //Копирование
  ~TextNode(); //Деструктор

  TextNode* GetNext(); //Получить нехт
  TextNode* GetDown(); //Получить довн
  char GetC(); //Получить символ
  int GetLevel(); //Получить уровень

  void SetNext(TextNode* _next); //Установить нехт
  void SetDown(TextNode* _down); //Установить довн
  void SetC(char _c); //Установить символ
  void SetLevel(int _l); //Установить уровень

  static void InitMem(int _size = 10); //Инициализация памяти и перепаковка
  static void FreeMem(); //Осовобождение памяти
  
  void* operator new(size_t size);
  void operator delete(void* p);

  friend std::ostream& operator<<(std::ostream& o, TextNode& t);
};

TextNode* TextNode::firstFree = 0;
TextNode* TextNode::textNodes = 0;
TextNode* TextNode::textNodes_tmp = 0;

std::ostream& operator<<(std::ostream& o, TextNode& t)
{
  if (t.level == 3)
  {
    o << t.c;
    if (t.next != nullptr)
      o << *(t.next);
  }
  else
  {
    if (t.down != nullptr)
      o << *(t.down);

    if (t.next != nullptr)
      o << *(t.next);
  }

  return o;
}

TextNode::TextNode(int _l, char _c)
{
  next = nullptr;
  down = nullptr;
  c = _c;
  level = _l;
}

TextNode::TextNode(char* _c, int l) //Принимает слово
{
  if (l < 1 || l > 3)
    throw - 1;

  if (l == 1) {
    next = nullptr;
    c = 0;
    level = l;
    int size = strlen(_c); //Получаем длину принятого слова

    if (size > 0)
    {
      int start = 0; 
      TextNode* wcur = new TextNode(2);
      this->SetDown(wcur);

      for (int i = 0; i < size; i++)
      {
        if (_c[i] == ' ' || i == size - 1) //Если пробел или конец
        {
          if (start != 0) 
          {
            wcur->SetNext(new TextNode(2)); //Создаем нехт
            wcur = wcur->GetNext(); //Получаем нехт
          }

          TextNode* lcur = new TextNode(3, _c[start]); //Создаем ЛКУР с символом
          wcur->SetDown(lcur); //Кладем его в довн

          for (int j = start + 1; j <= i; j++) 
          {
            //Записываем символы в нехт лкур
            lcur->SetNext(new TextNode(3, _c[j]));
            lcur = lcur->GetNext();
          }

          lcur->SetNext(nullptr);
          start = i + 1;
        }
      }
      wcur->SetNext(nullptr);
    }
  }

  if (l == 2) {
    next = nullptr;
    c = 0;
    level = l;
    int size = strlen(_c); //Получаем длину
    if (size > 0) 
    {
      TextNode* lcur = new TextNode(3, _c[0]); //Создаем текстнод с символом
      this->SetDown(lcur); //Кладем его сюда

      for (int i = 1; i < size; i++) 
      {
        lcur->SetNext(new TextNode(3, _c[i]));
        lcur = lcur->GetNext();
      }
      lcur->SetNext(nullptr);
    }
  }

  if (l == 3) 
  {
    c = _c[0]; //Записываем символ
    level = l; //Записываем уровень
    next = nullptr;
    down = nullptr;
  }
}

TextNode::TextNode(const TextNode& _node)
{
  next = nullptr;
  down = nullptr;
  c = _node.c;
  level = _node.level;
}

TextNode::~TextNode()
{
}

TextNode* TextNode::GetNext()
{
  return next;
}

TextNode* TextNode::GetDown()
{
  return down;
}

char TextNode::GetC()
{
  return c;
}

int TextNode::GetLevel()
{
  return level;
}

void TextNode::SetNext(TextNode* _next)
{
  next = _next;
}

void TextNode::SetDown(TextNode* _down)
{
  down = _down;
}

void TextNode::SetC(char _c)
{
  c = _c;
}

void TextNode::SetLevel(int _l)
{
  if (_l >= 1 && _l <= 3)
  {
    level = _l;
  }
  else
  {
    throw "Error";
  }
}

void TextNode::InitMem(int _size) //Выделение памяти
{
  if (_size <= 0)
    throw "Error";

  if ((firstFree == 0) && (textNodes == 0)) //Первое выделение памяти
  {
    textNodes = new TextNode[_size];
    firstFree = textNodes;
  }
  else if ((firstFree != 0) && (textNodes == 0))
  {
    throw "Error";
  }
  else //Добавить памяти
  {
    textNodes_tmp = new TextNode[_size];  //Создаем временное хранилище

    for (int i = 0; i < _size; i++)
    {
      textNodes_tmp[i] = (textNodes[i]); //Переписываем
    }

    textNodes = new TextNode[_size * 2]; //Выделяем больше памяти
    firstFree = textNodes;

    for (int i = 0; i < _size; i++)
    {
      textNodes[i] = textNodes_tmp[i]; //Возвращаем 
    }

    delete[] textNodes_tmp; //Удаляем временный массив
    textNodes_tmp = 0; //Зануляем
  }
}

void TextNode::FreeMem() //Освобождение памяти
{
  if (textNodes != 0)
  {
    delete[] textNodes;
  }
  textNodes = 0;
  firstFree = 0;
}

void* TextNode::operator new(size_t size)
{
  if ((firstFree == 0) && (textNodes == 0))
    InitMem();

  if ((firstFree == 0) && (textNodes != 0)) //Вызвать довыделение памяти
    InitMem();

  TextNode* node = firstFree;
  firstFree = firstFree->next;

  node->next = 0;

  return node;
}

void TextNode::operator delete(void* p)
{
  TextNode* node = (TextNode*)p;

  if ((firstFree == 0) && (textNodes == 0))
    throw "Error";

  node->next = nullptr;
  node->down = nullptr;
  node->c = 0;
  node->level = 3;

  node->next = firstFree;
  firstFree = node;
}