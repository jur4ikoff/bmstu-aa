# from src.database.base import
from src.schemas.schemas import FullUserData
from src.database.dao import UserDao
from src.database.schemas import InsertUser
import asyncio


class DataBaseService:
    def __init__(self):
        pass

    async def register_users(self, users: list[FullUserData]):
        # user_dao = UserDao()
        tasks = []
        for full_user_data in users:
            user = InsertUser(
                login=full_user_data.login,
                admin=full_user_data.admin,
                firstname=full_user_data.firstname,
                lastname=full_user_data.lastname,
                mail=full_user_data.mail,
                university_card=full_user_data.university_card,
                study_group=full_user_data.study_group,
                redmine_id=full_user_data.redmine_id,
            )
            task = asyncio.create_task(UserDao.add(user))
            tasks.append(task)

        await asyncio.gather(**tasks)
