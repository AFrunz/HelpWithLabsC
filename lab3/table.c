//#include "table.h"
#include "keySpace1.h"
#include "keySpace2.h"
#include <stdlib.h>

/*- 1) включение нового элемента в таблицу с соблюдением ограничений на уникальность
ключей в соответствующих ключевых пространствах и уникальности составного ключа (key1, key2);
2) поиск в таблице элемента, заданного составным ключом;
3) удаление из таблицы элемента, заданного составным ключом;
4) поиск в таблице элемента по любому заданному ключу; результатом поиска должна быть копии всех найденных элементов со значениями ключей; *(дополнительно) поиск может быть реализован как итератор одним из возможных способов, например, в этом случае метод возвращает один из возможных элементов, повторный вызов следующий элемент и т.д.
5) удаление из таблицы всех элементов, заданного ключом в одном из ключевых пространств;
6) вывод содержимого таблицы на экран (или текстовый файл); при этом формат вывода должен соответствовать приведенной выше структуре элемента таблицы;
 *
 *
 *
 *
 *
 *
 *
 */

typedef struct Table {
    KeySpace1 *ks1;         /* указатель на первое пространство ключей */
    KeySpace2 **ks2;
    int csize;
    int msize1;
    int msize2;

}Table;

typedef enum ans {
    KEY_NF = -1,
    TABLE_FULL = 1,
    ST_OK = 0,
    EL_NOTFOUND = 2,
    VERS_NOTFOUND = 3,
    KEY_AE = 4,
    KEY2_AE = 5
}ans;


Table* table_Init(int size1, int size2){
    // Инициаллизация таблицы
    Table* table = (Table*)calloc(1, sizeof(Table));
    table->ks1 = ks1_Init(size1);
    table->ks2 = ks2_Init(size2);
    table->msize1 = size1;
    table->msize2 = size2;
    return table;
}

int table_Add(char* key1, unsigned int key2, Table* t, char* first, char* second){
    if (ks2_Find(key2, t->ks2, t->msize2)) return KEY2_AE;

}




