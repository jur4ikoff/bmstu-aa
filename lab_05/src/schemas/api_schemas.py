from pydantic import BaseModel
from src.schemas.schemas import User
from typing import List


class RedmineGetUsersResponse(BaseModel):
    users: List[User]
    total_count: int = 0
    offset: int = 0


class RedmineCreateIssueRequest(BaseModel):
    project_id: str = "rasp"
    tracker_id: str = "tracker"
    subject: str
    description: str = "Base description"
    assigned_to_id: int
    start_date: str  # YYYY-MM-DD
    due_date: str  # YYYY-MM-DD
