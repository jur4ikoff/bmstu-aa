import logging

# ANSI escape-коды
RESET = "\033[0m"
COLORS = {
    "DEBUG": "\033[35m",  # фиолетовый
    "INFO": "\033[32m",  # зелёный
    "WARNING": "\033[33m",  # жёлтый
    "ERROR": "\033[31m",  # красный
    "CRITICAL": "\033[31m",
}


class ColorFormatter(logging.Formatter):
    def format(self, record: logging.LogRecord) -> str:
        color = COLORS.get(record.levelname, "")
        message = super().format(record)
        return f"{color}{message}{RESET}"


def setup_logging(level=logging.DEBUG) -> None:
    handler = logging.StreamHandler()
    formatter = ColorFormatter("%(asctime)s [%(levelname)s] %(name)s - %(message)s")
    handler.setFormatter(formatter)

    logging.getLogger("faker").setLevel(logging.CRITICAL + 1)
    root = logging.getLogger()
    root.setLevel(level)
    root.handlers.clear()
    root.addHandler(handler)