SELECT xmlelement(name export,
  xmlelement(name statistics,
    xmlforest( /* tworzy wiele elementów XML na raz na podstawie kolumn */
      (SELECT count(*) FROM patients) AS patientCount,
      (SELECT count(*) FROM studies) AS studyCount,
      (SELECT count(*) FROM series) AS seriesCount,
      (SELECT count(*) FROM images) AS imageCount
    )
  ),
  xmlelement(name patients,
    xmlagg( /* agreguje wielu pacjentów w zbiór pacjent */
      xmlelement(name patient,
        xmlattributes(p.id as id, p.patient_id as patient_id), /* dodanie atrybutów do znacznika patient */
        xmlforest(p.name as name, p.age as age, p.sex as sex), /* dodanie tagów wewn */
        (
          SELECT xmlagg(
            xmlelement(name study,
              xmlattributes(s.study_uid as study_uid),
              xmlforest(
                s.study_date as date,
                s.study_time as time,
                s.description as description,
                s.referring_physician as referring_physician,
                s.accession_number as accession_number,
                s.institution_name as institution_name,
                s.institution_address as institution_address
              ),
              (
                SELECT xmlagg(
                  xmlelement(name series,
                    xmlattributes(se.series_uid as series_uid),
                    xmlforest(
                      se.modality as modality,
                      se.description as description,
                      se.series_number as series_number
                    ),
                    (
                      SELECT xmlagg(
                        xmlelement(name image,
                          xmlforest(
                            i.rows,
                            i.columns,
                            i.bits_allocated,
                            i.instance_number,
                            i.image_type
                          )
                        )
                      )
                      FROM images i
                      WHERE i.series_id = se.id
                    )
                  )
                )
                FROM series se
                WHERE se.study_id = s.id /* FILTER_SERIES */
              )
            )
          )
          FROM studies s
          WHERE s.patient_id = p.id /* FILTER_STUDIES */
        )
      )
    )
  )
)
FROM patients p
WHERE 1=1 /* FILTER_PATIENTS */;
