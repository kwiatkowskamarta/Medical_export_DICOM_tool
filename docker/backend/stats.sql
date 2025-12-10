SELECT COUNT(*) AS total_patients FROM patients;
SELECT COUNT(*) AS total_studies FROM studies;
SELECT COUNT(*) AS total_series FROM series;
SELECT COUNT(*) AS total_images FROM images;

SELECT MIN(study_date) AS earliest_study, MAX(study_date) AS latest_study FROM studies;

SELECT 
    EXTRACT(YEAR FROM study_date) AS year,
    COUNT(*) AS study_count
FROM studies
GROUP BY year
ORDER BY study_count DESC;

