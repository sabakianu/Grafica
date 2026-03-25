data <- read.csv("mpg.csv", na.strings = "?") #curata ?

clean_data <- na.omit(data) #elimina randurile incomplete

model <- lm(mpg ~ horsepower + I(horsepower^2), data = clean_data) #calculeaza regresia

print("coeficienti:") #coeficientii
coef(model)