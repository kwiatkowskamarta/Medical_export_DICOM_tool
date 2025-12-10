\o export.xml
SELECT xmlelement(name export,
  -- Sekcja pacjentów z hierarchią
  xmlelement(name patients,
    xmlagg(
      xmlelement(name patient,
        xmlattributes(p.id as id, p.patient_id as patient_id),
        xmlforest(p.name as name, p.age as age, p.sex as sex),
        (
          SELECT xmlagg(
            xmlelement(name study,
              xmlattributes(s.study_uid as study_uid),
              xmlforest(s.study_date as date, s.description as description, s.referring_physician as referring_physician),
              (
                SELECT xmlagg(
                  xmlelement(name series,
                    xmlattributes(se.series_uid as series_uid),
                    xmlforest(se.modality as modality, se.description as description),
                    (
                      SELECT xmlagg(
                        xmlelement(name image,
                          xmlforest(i.rows, i.columns, i.bits_allocated)
                        )
                      )
                      FROM images i
                      WHERE i.series_id = se.id
                    )
                  )
                )
                FROM series se
                WHERE se.study_id = s.id
              )
            )
          )
          FROM studies s
          WHERE s.patient_id = p.id
        )
      )
    )
  ),

  -- Sekcja statystyk
  xmlelement(name statistics,
    xmlforest(
      (SELECT count(*) FROM patients) AS patientCount,
      (SELECT count(*) FROM studies) AS studyCount,
      (SELECT count(*) FROM series) AS seriesCount,
      (SELECT count(*) FROM images) AS imageCount,
      (SELECT coalesce(sum(octet_length(image_data)), 0) FROM images) AS totalSizeBytes
    )
  )
);
\o
