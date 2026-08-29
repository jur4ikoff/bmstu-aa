from src.database.models import User
from src.database.base import async_session_maker

from sqlalchemy import (
    update as sqlalchemy_update,
    delete as sqlalchemy_delete,
    insert as sqlalchemy_insert,
)
from sqlalchemy.exc import SQLAlchemyError
from sqlalchemy.future import select
from sqlalchemy.orm import joinedload


class BaseDAO:
    model = None

    @classmethod
    # async def find_all(cls, **filter_by):
    #     async with async_session_maker() as session:
    #         query = select(cls.model).filter(**filter_by)
    #         students = await session.execute(query)
    #         return students.scalars().all()

    # @classmethod
    # async def find_one_none_by_id(cls, data_id: int):
    #     async with async_session_maker() as session:
    #         query = select(cls.model).filter_by(id=data_id)
    #         result = await session.execute(query)

    #         return result.scalar_one_or_none()

    # @classmethod
    # async def find_one_or_none(cls, **filter_by):
    #     async with async_session_maker() as session:
    #         query = select(cls.model).filter_by(**filter_by)

    #         result = await session.execute(query)
    #         return result.scalar_one_or_none()

    @classmethod
    async def add(cls, **values):
        async with async_session_maker() as session:
            async with session.begin():
                new_instance = cls.model(**values)
                session.add(new_instance)

                try:
                    await session.commit()
                except SQLAlchemyError as e:
                    await session.rollback()
                    raise e
                return new_instance

    # @classmethod
    # async def update(cls, filter_by, **values):
    #     """определяет асинхронный метод update, который принимает два параметра:
    #     filter_by (условия фильтрации) и values (значения для обновления)."""

    #     async with async_session_maker() as session:  # Создание сессии
    #         async with session.begin():  # начало транзакции
    #             query = (
    #                 sqlalchemy_update(cls.model)
    #                 .where(*[getattr(cls.model, k) == v for k, v in filter_by.items()])
    #                 .values(**values)
    #                 .execution_options(synchronize_session="fetch")
    #             )

    #             # .execution_options(synchronize_session="fetch"): Опция, чтобы синхронизировать состояние сессии с базой данных после выполнения запроса.

    #             result = await session.execute(query)

    #             try:
    #                 await session.commit()
    #             except SQLAlchemyError as e:
    #                 await session.rollback()
    #                 raise e

    #             return result.rowcount

    # @classmethod
    # async def delete(cls, delete_all: bool = False, **filter_by):
    #     if not delete_all and not filter_by:
    #         raise ValueError("Необходимо указать хотя бы один параметр")

    #     async with async_session_maker() as session:
    #         async with session.begin():
    #             query = sqlalchemy_delete(cls.model).filter_by(**filter_by)
    #             result = await session.execute(query)

    #             try:
    #                 await session.commit()
    #             except SQLAlchemyError as e:
    #                 await session.rollback()
    #                 raise e

    #             return result.rowcount


class UserDao(BaseDAO):
    model = User
    
