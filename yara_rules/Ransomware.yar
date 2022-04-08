import "pe"

rule nastyware_branch1
{
    meta:
        description = "Rule based in branch 1 from DAMICORE result of Nastyware study."
        author = "Nilson Sangy"
        date = "2022-04-05"
        rule_version = "v1.0"
        malware_type = "ransomware"

    strings:
        $s1 = "Microsoft (R) Windows (R) Operating System"
    
    condition:
        (pe.imports("mscoree.dll", "_CorExeMain") and not $s1)
    
}


// Improving this rule. With 42 false positives.
rule nastyware_branch2
{
    meta:
        description = "Rule based in branch 2 from DAMICORE result of Nastyware study."
        author = "Nilson Sangy"
        date = "2022-04-05"
        rule_version = "v1.0"
        malware_type = "ransomware"

    condition:
        ( pe.imports("kernel32.dll", "GetProcAddress") and pe.imports("kernel32.dll", "ExitProcess") )
        and not
        (   pe.imports("msvcrt.dll", "_amsg_exit")
            or pe.imports("msvcrt.dll", "__C_specific_handler")
            or pe.imports("WS2_32.dll", "recv")
            or pe.imports("api-ms-win-core-libraryloader-l1-2-0.dll", "DisableThreadLibraryCalls") )
            
    
}


rule nastyware_branch3
{
    meta:
        description = "Rule based in branch 3 from DAMICORE result of Nastyware study."
        author = "Nilson Sangy"
        date = "2022-04-05"
        rule_version = "v1.0"
        malware_type = "ransomware"

    strings:
        $s1 = "adal.dll"
        $s2 = "vm3dgl64"
        $s3 = "vm3dservice"
    
    condition:
        ( pe.imports("kernel32.dll", "ExitProcess") and pe.imports("kernel32.dll", "RtlUnwind") and not any of them )

}
