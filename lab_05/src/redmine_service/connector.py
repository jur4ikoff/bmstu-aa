import aiohttp
from typing import Dict, Any, List
from src.config import settings, get_url_to_redmine
from src.redmine_service.client import RedmineClient
import logging

logger = logging.getLogger(__name__)


class RedmineConnector:
    """
    Отвечает за непосредственное взаимодействие с Redmine API по HTTP.
    Ничего не знает о бизнес-логике, только отправляет данные.
    """

    def __init__(self):
        self.base_url = get_url_to_redmine()
        self.api_key = settings.REDMINE_TOKEN
        self.headers = {
            "Content-Type": "application/json",
            "X-Redmine-API-Key": self.api_key,
        }
        self.client = RedmineClient()

    async def create_user(
        self, session: aiohttp.ClientSession, user_payload: Dict[str, Any]
    ) -> str:
        url = f"{self.base_url}/users.json"
        payload = {"user": user_payload}

        try:
            async with session.post(
                url, json=payload, headers=self.headers
            ) as response:
                if response.status == 201:
                    logger.debug(f"SUCCESS adding {await response.text()}")
                    return user_payload.get("login")
                else:
                    error_text = await response.text()
                    raise ValueError(
                        f"ERROR: Failed to create {user_payload.get('login')}. Status: {response.status}. Msg: {error_text}"
                    )
        except Exception as e:
            logger.error(f"Ошибка {e}")

    async def get_users(self, offset=0, limit=10000) -> str:
        url = self.base_url + "/users.json"
        params = {
            "offset": offset,
            "limit": limit,
        }
        async with aiohttp.ClientSession() as session:
            try:
                async with session.get(
                    url, headers=self.headers, params=params
                ) as response:
                    if response.status == 200:
                        return await response.text()
                    else:
                        logger.error(
                            f"Error while getting users, status: {response.status}"
                        )
            except Exception as e:
                logger.error(f"Exception occured {e}")

    async def create_group(self, name: str, ids: list) -> str:
        url = self.base_url + "/groups.json"
        payload = {"group": {"name": name, "user_ids": ids}}

        try:
            async with aiohttp.ClientSession() as session:
                async with session.post(
                    json=payload, headers=self.headers, url=url
                ) as response:
                    if response.status == 201:
                        logger.info(f"SUCCESS creating group")
                        return await response.text()
                    else:
                        logger.error(
                            f"ERROR while creating group {await response.text()}"
                        )
        except Exception as e:
            logger.error(f"Exception occured {e}")

    async def create_issue(
        self,
        project_id: str,
        tracker_id: str,
        subject: str,
        description: str,
        assigned_to_id: int,
        start_date: str,
        due_date: str,
    ):
        url = self.base_url + "/issues.json"
        payload = {
            "issue": {
                "project_id": project_id,
                "tracker_id": tracker_id,
                "subject": subject,
                "description": description,
                "assigned_to_id": assigned_to_id,
                "start_date": start_date,
                "due_date": due_date,
            }
        }

        try:
            return await self.client.post_to_redmine(url, payload)
        except Exception as e:
            logger.error(f"Exception occured {e}")

    async def delete_user(self, redmine_id: int):
        url = self.base_url + f"/users/{redmine_id}.json"
        return await self.client.delete_request(url=url)
