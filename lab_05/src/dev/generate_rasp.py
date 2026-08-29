"""
Скрипт генерирует расписание
"""

import os
import asyncio
import random
import logging
from faker import Faker
from src.redmine_service.service import RedmineService
from src.schemas.api_schemas import RedmineCreateIssueRequest
from src.utils.logger import setup_logging

setup_logging()
logger = logging.getLogger(__name__)

SCRIPT_PATH = os.path.abspath(__file__)
SCRIPT_DIR = os.path.dirname(SCRIPT_PATH)

PROJECT_IDENT = "rasp"


def generator():
    fake = Faker()

    def generate_user(is_admin=False):
        language = "en"
        _fake = Faker("en_US")

        first_name = _fake.first_name()
        last_name = _fake.last_name()
        login = (first_name + "_" + last_name).lower().replace(" ", "_")
        mail = f"{first_name.lower()}.{last_name.lower()}@example.com"
        password = fake.password(
            length=8, special_chars=False, digits=True, upper_case=True, lower_case=True
        )
        admin = "true" if is_admin else "false"
        mail_notification = (
            "all"
            if is_admin
            else random.choice(["all", "only_my_events", "only_owner", "none"])
        )

        return {
            "login": login,
            "firstname": first_name,
            "lastname": last_name,
            "mail": mail,
            "password": password,
            "admin": admin,
            "language": language,
            "mail_notification": mail_notification,
        }

    users = [generate_user(is_admin=True)]
    logger.info("start generating_users")
    for _ in range(500):
        users.append(generate_user(is_admin=False))

    logger.info("finish generating users")

    return users


async def main():
    # users = generator()
    service = RedmineService()

    await service.create_issue(
        RedmineCreateIssueRequest(
            subject="test_task",
            assigned_to_id=1,
            start_date="2025-12-25",
            due_date="2025-12-26",
        )
    )
    # await service.register_users_batch(users)


if __name__ == "__main__":
    asyncio.run(main())
