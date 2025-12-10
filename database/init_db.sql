DROP TABLE IF EXISTS images;
DROP TABLE IF EXISTS series;
DROP TABLE IF EXISTS studies;
DROP TABLE IF EXISTS patients;

CREATE TABLE patients (
    id SERIAL PRIMARY KEY,
    patient_id TEXT NOT NULL UNIQUE,
    name TEXT,
    age INTEGER,
    sex CHAR(1)
);

CREATE TABLE studies (
    id SERIAL PRIMARY KEY,
    patient_id INTEGER NOT NULL REFERENCES patients(id) ON DELETE CASCADE,
    study_uid TEXT NOT NULL UNIQUE,
    study_date DATE,
    study_time TIME,
    description TEXT,
    referring_physician TEXT,
    accession_number TEXT,
    institution_name TEXT,
    institution_address TEXT
);

CREATE TABLE series (
    id SERIAL PRIMARY KEY,
    study_id INTEGER NOT NULL REFERENCES studies(id) ON DELETE CASCADE,
    series_uid TEXT NOT NULL UNIQUE,
    modality TEXT,
    description TEXT,
    series_number INTEGER
);

CREATE TABLE images (
    id SERIAL PRIMARY KEY,
    series_id INTEGER NOT NULL REFERENCES series(id) ON DELETE CASCADE,
    rows INTEGER,
    columns INTEGER,
    bits_allocated INTEGER,
    instance_number INTEGER,
    image_type TEXT
);

CREATE INDEX idx_studies_patient_id ON studies(patient_id);
CREATE INDEX idx_series_study_id ON series(study_id);
CREATE INDEX idx_images_series_id ON images(series_id);
