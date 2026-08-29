from src.database.base import Base

from sqlalchemy.ext.asyncio import create_async_engine, async_sessionmaker, AsyncAttrs
from sqlalchemy.exc import ResourceClosedError
from sqlalchemy import text, func
from sqlalchemy.orm import mapped_column, DeclarativeBase, declared_attr, sessionmaker
from datetime import datetime
from typing import Annotated
from sqlalchemy import String, Boolean, Integer
from sqlalchemy.orm import Mapped, mapped_column


class User(Base):
    login: Mapped[str] = mapped_column(String(64), primary_key=True)
    admin: Mapped[bool] = mapped_column(Boolean, default=False, nullable=False)
    firstname: Mapped[str] = mapped_column(String(100), nullable=False)
    lastname: Mapped[str] = mapped_column(String(100), nullable=False)
    mail: Mapped[str] = mapped_column(String(255), nullable=True)
    university_card: Mapped[str] = mapped_column(String(100), nullable=False)
    study_group: Mapped[str] = mapped_column(String(100), nullable=False)
    redmine_id: Mapped[int] = mapped_column(Integer, nullable=True)
