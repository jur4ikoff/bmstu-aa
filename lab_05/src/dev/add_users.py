"""
Скрипт генерирует пользователей в формате csv
login,firstname,lastname,mail,password,admin,language,mail_notification
"""

import csv
import os
import asyncio
import random
import logging
import string
from faker import Faker
from src.redmine_service.service import RedmineService
from src.utils.logger import setup_logging
from typing import List
from src.schemas.schemas import FullUserData, User, GenerateUser
from src.database.service import DataBaseService

setup_logging()
logger = logging.getLogger(__name__)

SCRIPT_PATH = os.path.abspath(__file__)
SCRIPT_DIR = os.path.dirname(SCRIPT_PATH)


def generator() -> List[GenerateUser]:
    fake = Faker()
    _fake = Faker("en_US")

    def generate_university_card() -> str:
        # AAAAxxx: 4 заглавные буквы, 3 цифры
        letters = "".join(random.choices(string.ascii_uppercase, k=4))
        digits = "".join(random.choices(string.digits, k=3))
        return letters + digits

    def generate_study_group() -> str:
        # IU7-XX: 2 цифры
        digits = "".join(random.choices(string.digits, k=2))
        return f"IU7-{digits}"


def generator() -> List[GenerateUser]:
    fake = Faker()
    _fake = Faker("en_US")

    def generate_university_card() -> str:
        # AAAAxxx: 4 заглавные буквы, 3 цифры
        letters = "".join(random.choices(string.ascii_uppercase, k=4))
        digits = "".join(random.choices(string.digits, k=3))
        return letters + digits

    def generate_study_group() -> str:
        # IU7-XX: 2 цифры
        digits = "".join(random.choices(string.digits, k=2))
        return f"IU7-{digits}"

    def generate_user(is_admin: bool = False) -> GenerateUser:
        first_name = _fake.first_name()
        last_name = _fake.last_name()

        login = (first_name + "_" + last_name).lower().replace(" ", "_")
        mail = f"{first_name.lower()}.{last_name.lower()}@example.com"

        password = fake.password(
            length=8,
            special_chars=False,
            digits=True,
            upper_case=True,
            lower_case=True,
        )

        admin = bool(is_admin)

        university_card = generate_university_card()
        study_group = generate_study_group()

        # Возвращаем GenerateUser вместо FullUserData, так как есть password
        return GenerateUser(
            redmine_id=None,
            login=login,
            admin=admin,
            firstname=first_name,
            lastname=last_name,
            mail=mail,
            university_card=university_card,
            study_group=study_group,
            password=password,
        )

    users = []

    # Можно добавить логирование, если нужно
    # logger.info("start generating users")
    for _ in range(1):
        users.append(generate_user(is_admin=False))

    return users


async def main():
    users = generator()
    redmine_service = RedmineService()
    database_service = DataBaseService()
    tasks = []

    redmine_task = asyncio.create_task(redmine_service.register_users_batch(users))
    database_task = asyncio.create_task(database_service.register_users(users))
    # tasks.append(redmine_task)
    tasks.append(database_task)

    results = await asyncio.gather(*tasks)


if __name__ == "__main__":
    asyncio.run(main())
