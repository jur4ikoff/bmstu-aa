import asyncio
import logging
import json
import aiohttp
from typing import List, Dict, Any
from src.redmine_service.connector import RedmineConnector
from src.schemas.api_schemas import RedmineGetUsersResponse, RedmineCreateIssueRequest
from src.schemas.schemas import User, GenerateUser, FullUserData

logger = logging.getLogger(__name__)
MAX_LIMIT = 100


class RedmineService:
    """
    Управляет процессом добавления пользователей.
    Принимает список данных и использует Connector для выполнения задач.
    """

    def __init__(self):
        self.connector = RedmineConnector()

    async def register_users_batch(self, users: List[FullUserData]):
        """
        Асинхронно регистрирует список пользователей.
        """
        async with aiohttp.ClientSession() as session:
            tasks = []

            for user in users:
                task = asyncio.create_task(
                    self.connector.create_user(
                        session,
                        user_payload={
                            "login": user.login,
                            "firstname": user.firstname,
                            "lastname": user.lastname,
                            "mail": user.mail,
                            "password": user.password,
                            "admin": user.admin,
                            "language": "en",
                            "mail_notification": "all",
                        },
                    )
                )
                tasks.append(task)

            results = await asyncio.gather(*tasks)
            return results

    async def delete_users(self, users: list[User]):
        tasks = []

        for user in users:
            if user.redmine_id == 1:
                continue
            task = asyncio.create_task(self.connector.delete_user(user.redmine_id))
            tasks.append(task)

        results = await asyncio.gather(*tasks)
        return results

    # async def get_users(self) -> GetUsersResponse:
    #     """Асинхроное получение пользователей"""
    #     json_text = await self.connector.get_users()
    #     data = json.loads(json_text)
    #     users_model = GetUsersResponse.model_validate(data)
    #     return users_model

    async def get_all_users(self) -> RedmineGetUsersResponse | None:
        first_json = await self.connector.get_users(offset=0, limit=MAX_LIMIT)
        try:
            first_data = json.loads(first_json)
            users_model = RedmineGetUsersResponse.model_validate(first_data)
        except Exception as e:
            logger.error(f"Impossible convert to users list. exception={e}")
            return

        total = users_model.total_count
        loaded = len(users_model.users)
        logger.info(f"Loaded first page: {loaded}/{total} users")

        if loaded >= total:
            return users_model

        offsets: List[int] = list(range(loaded, total, MAX_LIMIT))

        tasks = [
            asyncio.create_task(
                self.connector.get_users(offset=offset, limit=MAX_LIMIT)
            )
            for offset in offsets
        ]

        pages_json = await asyncio.gather(*tasks)
        for page_json in pages_json:
            data = json.loads(page_json)
            page_model = RedmineGetUsersResponse.model_validate(data)
            users_model.users.extend(page_model.users)
            logger.info(f"Loaded page: total users in model = {len(users_model.users)}")

        return users_model

    async def create_group(self, name: str, ids: list):
        return await self.connector.create_group(name, ids)

    async def create_issue(self, redmine_task: RedmineCreateIssueRequest):
        return await self.connector.create_issue(**redmine_task.model_dump())
