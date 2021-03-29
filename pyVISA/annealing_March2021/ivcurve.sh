# Irradiated NewSiPM 2e14

# +20 DEG (somewhere between)
#root -q -b ivcurve.C\(\"20200831_HDR2-2e14-2/p20C/light/HDR2-2e14-2-20C-11.36mA-Orange-1.csv\",\"20200831_HDR2-2e14-2/p20C/dark/HDR2-2e14-2-20C-dark-1.csv\"\); #Good
#root -q -b ivcurve.C\(\"20200831_HDR2-2e14-2/p20C/light/HDR2-2e14-2-20C-11.36mA-Orange-2.csv\",\"20200831_HDR2-2e14-2/p20C/dark/HDR2-2e14-2-20C-dark-1.csv\"\); #Good

#root -q -b ivcurve2.C\(\"fase1/outputdata_LED.csv\",\"fase1/outputdata_DARK.csv\"\); #Good
#root -q -b ivcurve2.C\(\"fase1/outputdata_LED_1.csv\",\"fase1/outputdata_DARK_1.csv\"\); #Good

#root -q -b ivcurve2.C\(\"fase1/outputdata_LED_2_AFTER.csv\",\"fase1/outputdata_DARK_2_AFTER.csv\"\); #Good
#root -q -b ivcurve2.C\(\"fase1/outputdata_LED_1_AFTER.csv\",\"fase1/outputdata_DARK_1_AFTER.csv\"\); #Good

#root -q -b ivcurve2.C\(\"fase2/outputdata_LED_2.csv\",\"fase2/outputdata_DARK_2.csv\"\); #Good
#root -q -b ivcurve2.C\(\"fase2/outputdata_LED_1.csv\",\"fase2/outputdata_DARK_1.csv\"\); #Good

#root -q -b ivcurve2.C\(\"fase3/outputdata_LED_2.csv\",\"fase3/outputdata_DARK_2.csv\"\); #Good
#root -q -b ivcurve2.C\(\"fase3/outputdata_LED_1.csv\",\"fase3/outputdata_DARK_1.csv\"\); #Good

#root -q -b ivcurve2.C\(\"fase4/outputdata_LED_2.csv\",\"fase4/outputdata_DARK_2.csv\"\); #Good
#root -q -b ivcurve2.C\(\"fase4/outputdata_LED_1.csv\",\"fase4/outputdata_DARK_1.csv\"\); #Good

#root  ivcurve2.C\(\"fase4/outputdata_LED_2_234C.csv\",\"fase4/outputdata_DARK_2_234C.csv\"\); #Good
#root ivcurve2.C\(\"fase4/outputdata_LED_1_234C.csv\",\"fase4/outputdata_DARK_1_234C.csv\"\); #Good

root ivcurve2.C\(\"fase5/outputdata_1_LED.csv\",\"fase5/outputdata_1_DARK.csv\"\); #Good
root ivcurve2.C\(\"fase5/outputdata_2_LED.csv\",\"fase5/outputdata_2_DARK.csv\"\); #Good
