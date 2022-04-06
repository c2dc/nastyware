import "pe"

rule bad_imports
{
    meta:
        description = "Ransomwares hunting rule"
        author = "Nilson Sangy"
        date = "2022-04-05"
        rule_version = "v1.0"
        malware_type = "ransomware"

    condition:
        pe.imports("mscoree.dll", "_CorExeMain") and pe.number_of_imported_functions == 1
}
