# Конвееры, ЛР5
## Задание с Redmine


## Запуск
```bash
# Подготовка
git clone <link>
cd pyua23u362
python -m venv .venv
source ./.venv/bin/activate

# Установка зависимостей
pip install poetry
poetry lock
poetry install --no-root

# Запуск redmine и постгри с нуля
make up
```
Далее ручками заходим в redmine `http://localhost:8080`
- меняем пароль админа
- берем PAT token, добалеяем в .env

## Миграции для бд
```bash
cd src
alembic init -t async migration
mv alembic.ini ../alembic.ini

# необходимо поменять несколько строк
# alembic.ini
# меняем
script_location = %(here)s/src/migration


# Добавить строки в .env
config.set_main_option("sqlalchemy.url", DATABASE_URL)
target_metadata = Base.metadata

# в migration/.env 
import sys
from os.path import dirname, abspath


sys.path.insert(0, dirname(dirname(abspath(__file__))))


from src.database.base import DATABASE_URL, Base
from src.database.models import User


# делаем миграцию
cd ../
alembic revision --autogenerate -m "Initial revision"
alembic upgrade head
```

```bash
make init
```

```
- создаем проект, называем `raspisanie`, идентификатор `rasp`
- Создем роль `Developer`
- добавляем в проект группу `aa5`, с ролью `Developer`
- Создаем статусы todo и done
- Создаем трекеры с названием tracker, ассигним на проект
- Делаем последовательность действий для трекера
- Создать приоритет задачи `Normal`

Для генерации расписания
```bash
make generate_rasp
# python -m src.dev.generate_rasp
```