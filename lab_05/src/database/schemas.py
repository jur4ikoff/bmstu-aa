from pydantic import BaseModel


class InsertUser(BaseModel):
    login: str
    admin: bool
    firstname: str
    lastname: str
    mail: str | None = None
    university_card: str
    study_group: str
    redmine_id: int | None = None
