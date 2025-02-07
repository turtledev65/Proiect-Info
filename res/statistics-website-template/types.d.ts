type Ethnicity = {
  type: number;
  name: string;
  total: number;
};

type Area = {
  name: string;
  totalPopulation: number;
  historicalRegion: string;
  ethnicities: Ethnicity[];
};

type ChartItem = {
  label: string;
  value: number;
  color: string;
}