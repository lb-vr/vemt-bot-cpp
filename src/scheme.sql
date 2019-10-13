DROP TABLE IF EXISTS answers;
DROP TABLE IF EXISTS question_choices;
DROP TABLE IF EXISTS question_items;

DROP TABLE IF EXISTS results;
DROP TABLE IF EXISTS submissions;
DROP TABLE IF EXISTS entries;

CREATE TABLE entries (
        id INTEGER NOT NULL,
        discord_user_id INTEGER NOT NULL,
        current_phase INTEGER NOT NULL,
        query_status_message_id INTEGER NULL DEFAULT NULL,
        working_status_message_id INTEGER NULL DEFAULT NULL,
        created_at DATETIME DEFAULT (CURRENT_TIMESTAMP) NOT NULL,
        updated_at DATETIME DEFAULT (CURRENT_TIMESTAMP) NOT NULL,
        PRIMARY KEY (id)
);

CREATE TABLE submissions (
        id INTEGER NOT NULL,
        entry_id INTEGER NOT NULL,
        package_url TEXT NOT NULL,
        current_phase INTEGER NOT NULL,
        created_at DATETIME DEFAULT (CURRENT_TIMESTAMP) NOT NULL,
        updated_at DATETIME DEFAULT (CURRENT_TIMESTAMP) NOT NULL,
        PRIMARY KEY (id),
        FOREIGN KEY(entry_id) REFERENCES entries (id)
);

CREATE TABLE results (
        id INTEGER NOT NULL,
        submission_id INTEGER NOT NULL,
        as_phase INTEGER NOT NULL,
        is_passed BOOLEAN NOT NULL,
        log_text TEXT NOT NULL,
        created_at DATETIME DEFAULT (CURRENT_TIMESTAMP) NOT NULL,
        PRIMARY KEY (id),
        FOREIGN KEY(submission_id) REFERENCES submissions (id),
        CHECK (is_passed IN (0, 1))
);

CREATE TABLE question_items (
        id INTEGER NOT NULL,
        title TEXT NOT NULL,
        detail TEXT NOT NULL,
        valid_type TEXT NOT NULL,
        regex TEXT NOT NULL,
        max_length INTEGER NULL DEFAULT NULL,
        required_when_phase INTEGER NOT NULL,
        required_when_timepoint DATETIME NOT NULL,
        allow_multiline BOOLEAN NOT NULL,
        is_required BOOLEAN NOT NULL,
        created_at DATETIME DEFAULT (CURRENT_TIMESTAMP) NOT NULL,
        PRIMARY KEY (id),
        CHECK (allow_multiline IN (0, 1)),
        CHECK (is_required IN (0, 1))
);

CREATE TABLE question_choices (
        id INTEGER NOT NULL,
        question_item_id INTEGER NOT NULL,
        title TEXT NOT NULL,
        created_at DATETIME DEFAULT (CURRENT_TIMESTAMP) NOT NULL,
        PRIMARY KEY (id),
        FOREIGN KEY(question_item_id) REFERENCES question_items (id)
);

CREATE TABLE answers (
        id INTEGER NOT NULL,
        entry_id INTEGER NOT NULL,
        question_item_id INTEGER NOT NULL,
        item_value TEXT NULL DEFAULT NULL,
        created_at DATETIME DEFAULT (CURRENT_TIMESTAMP) NOT NULL,
        updated_at DATETIME DEFAULT (CURRENT_TIMESTAMP) NOT NULL,
        PRIMARY KEY (id),
        FOREIGN KEY(entry_id) REFERENCES entries (id),
        FOREIGN KEY(question_item_id) REFERENCES question_items (id)
);

BEGIN;
INSERT INTO entries (id, discord_user_id)
VALUES
        (1, 100)
;
INSERT INTO submissions (id, entry_id, package_url, current_phase)
VALUES
        (1, 100, "https://external.storage.example.com/user100/package", 20)
;
INSERT INTO question_items (id, title, detail, valid_type, regex, max_length, required_when_phase, required_when_timepoint, allow_multiline, is_required)
VALUES
        (1, "author", "enter your name", 1, "[a-zA-Z0-9 ]+", 64, 3, '2019-12-01 00:00:00', 0, 1),
        (2, "size",   "your booth size", 1, "",               1, 2, '2019-11-01 00:00:00', 0, 1)
;
INSERT INTO question_choices (question_item_id, title)
VALUES
        (2, "4 x 4 x 5"),
        (2, "4 x 8 x 5"),
        (2, "8 x 8 x 3")
;
COMMIT;
