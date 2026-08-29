import aiohttp
from typing import Dict, Any, List
from src.config import settings, get_url_to_redmine
import logging

logger = logging.getLogger(__name__)


class RedmineClient:
    def __init__(self):
        self.api_key = settings.REDMINE_TOKEN
        self.headers = {
            "Content-Type": "application/json",
            "X-Redmine-API-Key": self.api_key,
        }

    async def post_to_redmine(self, url: str, payload: dict[str, Any]):
        async with aiohttp.ClientSession() as session:
            async with session.post(
                json=payload, headers=self.headers, url=url
            ) as response:
                # logger.info(f"[Redmine Client] SUCCESS")
                logger.debug(f"response status_code: {response.status}")
                return await response.text()
            # else:
            # logger.error(f"[Redmine Client] ERROR {await response.text()}")

    async def delete_request(self, url: str):
        async with aiohttp.ClientSession() as session:
            async with session.delete(url=url, headers=self.headers) as response:
                if response.status == 201:
                    logger.info(f"[Redmine Client] SUCCESS")
                    return await response.text()
                else:
                    logger.error(f"[Redmine Client] ERROR {await response.text()}")
