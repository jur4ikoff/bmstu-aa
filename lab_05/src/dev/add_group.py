"""Скрипт для генерации группы пользователей"""

import os
import asyncio
import logging
from src.redmine_service.service import RedmineService
from src.utils.logger import setup_logging

setup_logging()
logger = logging.getLogger(__name__)

SCRIPT_PATH = os.path.abspath(__file__)
SCRIPT_DIR = os.path.dirname(SCRIPT_PATH)


async def main():
    service = RedmineService()
    users = await service.get_all_users()
    logger.info(f"ALL users reading, {users.total_count}")

    ids = []
    for user in users.users:
        ids.append(user.redmine_id)

    await service.create_group("aa5", ids)


if __name__ == "__main__":
    asyncio.run(main())
