# Результаты тестирования
## Соответствие требований и тестов:
| Функциональные требования         | ID теста           |
| ------------- |------------------|
| Функция шифрования файла  | 001 |
| Функция дешифрования файла    | 002 |
| Функция удаления файла | 003 |
| Функция копирования файла  | 004 |
| Функция добавления книги    | 005 |

| Сценарий         | Действие           | Ожидаемый результат         | Фактический результат           | Оценка         |
| ---------------- |------------------- | ----------------------------|---------------------------------|----------------|
| 001-1: Шифрование файла. Успешное шифрование. | 1. Создать любой файл. 2. Выполнить шифрование (Ctrl+Shift+E). | Повление диалогового окна с сообщением об успешном выполнении операции | Повление диалогового окна с сообщением об успешном выполнении операции | Сценарий выполнен |
| 001-2: Шифрование файла. ReadOnly файл. | (В Проводнике) 1. Создать любой файл. 2. ПКМ по файлу -> Свойства -> Отметить атрибут "Только чтение" -> Применить 3. Выполнить шифрование. | Появление диалогового окна с сообщением о невозможности выполнения операции | Появление диалогового окна с сообщением о невозможности выполнения операции | Сценарий выполнен |
| 001-3: Шифрование файла. Защифрованный файл. | 1. Создать файл 2. Выполнить успешное шифрование. 3. Ещё раз выполнить шифрование этого же файла. | Появление диалогового окна с сообщением о том, что файл уже зашифрован | Появление диалогового окна с сообщением о том, что файл уже зашифрован | Сценарий выполнен |
| 001-4: Шифрование файла. Получение доступа другим пользователем | 1. Выполнить успешное шифрование файла 2. Зайти в систему с помощью другой учётной записи пользователя 3. Попытаться получить доступ к защифрованному файлу. | Отсутствие доступа к файлу. Сообщение об отсутсвтии прав доступа. | Отсутствие доступа к файлу. Приложение, открывающее файл выводит сообщение об отсутсвтии прав доступа. | Сценарий выполнен |
| 002-1: Дешифрование файла. Успешная дешифрация. | 1. Создать файл. 2. Выполнить успешное шифрование файла. 3. Выполнить дешифрование файла (Ctrl+Shift+D) | Диалоговое окно с сообщением об успешном выполнении. | Диалоговое окно с сообщением об успешном выполнении. | Сценарий выполнен |
| 002-2: Дешифрование файла. Нешифрованный файл. | 1. Создать файл. 2. Выполнить дешифрацию этого файла | Диалоговое окно с сообщением об невозможности выполнения операции. | Диалоговое окно с сообщением об невозможности выполнения операции. | Сценарий выполнен |
| 002-3: Дешифрование файла. Доступ к файлу другим пользователем. | 1. Создать и зашифровать файл. 2. Убедиться, что он недоступен другому пользователю. 3. Дешифровать файл. 4. Попытаться получить доступ к файлу | Доступ как и до шифрации файла. | Доступ как и до шифрации файла. | Сценарий выполнен |
| 003-1: Удаление файла. Успешное удаление. | 1. Выбрать файл 2. Выполнить удаление (Delete) | Диалоговое окно с сообщением об успешном выполнении операции. Отсутствие файла текущей директории. | Диалоговое окно с сообщением об успешном выполнении операции. Отсутствие файла текущей директории. | Сценарий выполнен |
| 003-2: Удаление файла. Отсутствие прав доступа. | 1. Выбрать файл 2. Выполнить удаление | Диалоговое окно с сообщением об отказе в доступе. | Нет отображения сообщения об успешном выполнении | Сценарий не выполнен |
| 003-3: Удаление файла. Отсутствие файла после удаления. | 1. Выбрать файл 2. Выполнить удаление. | Обновление списка файлов директории. Отсутствие удалённого файла в списке (в том числе в скрытых файлах). | Обновление списка файлов директории. Отсутствие удалённого файла в списке (в том числе в скрытых файлах). | Сценарий выполнен |
| 004-1: Копирование файла. Успешное выполнение. | 1. Выбор файла. 2. Инициирование операции копирования (Ctrl+C). 3. Выбор директории-приёмника. 4. Вставка копии файла (Ctrl+V). | Диалоговое окно с сообщением об успешном выполнении операции. | Диалоговое окно с сообщением об успешном выполнении операции. | Сценарий выполнен |
| 004-2: Копирование файла. Дублирование имён. | 1. Выбор файла. 2. Инициирование операции копирования (Ctrl+C). 3. Выбор директории-приёмника. 4. Вставка копии файла (Ctrl+V). | Диалоговое окно с сообщением о том, что файл с таким именем уже существует. | Диалоговое окно с сообщением о том, что файл с таким именем уже существует. | Сценарий выполнен |
| 004-3: Копирование файла. Наличие нового экземпляра файла. | 1. Выбор файла. 2. Инициирование операции копирования (Ctrl+C). 3. Выбор директории-приёмника. 4. Вставка копии файла (Ctrl+V). | Диалоговое окно с сообщением об успешном выполнении операции. Наличие файла как в директории-приёмнике, так и в первоначальной директории. | Диалоговое окно с сообщением об успешном выполнении операции. Наличие файла как в директории-приёмнике, так и в первоначальной директории. | Сценарий выполнен |
| 005-1: Отображение скрытых файлов. Отображение "Скрытого" файла. | (В Проводнике) 1. Выбор файла. 2. ПКМ по файлу -> Свойства -> Отметить атрибут "Скрытый" -> Применить. | При включенном режиме отображения скрытых файлов (Ctrl+H) файл отображается в списке файлов директории, в которой он расположен, при выключенном режиме - не отображается. | При включенном режиме отображения скрытых файлов (Ctrl+H) файл отображается в списке файлов директории, в которой он расположен, при выключенном режиме - не отображается. | Сценарий выполнен |
| 005-2: Отображение скрытых файлов. Снятие атрибута "Скрытый" файла. | (В Проводнике в режиме просмотра скрытых файлов) 1. Выбор скрытого файла. 2. ПКМ по файлу -> Свойства -> Снять атрибут "Скрытый" -> Применить. | Файл отображается в списке файлов директории, в которой он расположен, независимо от режима отображения скрытых файлов. | Файл отображается в списке файлов директории, в которой он расположен, независимо от режима отображения скрытых файлов. | Сценарий выполнен |

Успешно пройденные тесты: 14/15.
Отсутсвовали сценарии для тестирования нефункциональных требований.
Пожелания к проекту: более подробные сообщения об ошибках. Добавить окна прогресса операций копирования, перемещения, удаления. Добавить операцию переименования.
