from pydantic import BaseModel


class User(BaseModel):
    redmine_id: int | None = None
    login: str
    admin: bool
    firstname: str
    lastname: str
    mail: str


class FullUserData(User):
    university_card: str
    study_group: str

class GenerateUser(FullUserData):
    password: str