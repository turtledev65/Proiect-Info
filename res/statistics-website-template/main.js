class Popup {
  constructor() {
    const container = document.createElement("div");
    container.classList.add("popup");
    document.body.appendChild(container);

    const title = document.createElement("p");
    title.innerText = "This is a title";
    title.classList.add("popup-title");
    container.appendChild(title);

    const { table, dataCells } = createTable(["Regiune Istorica", "Populatie"]);
    table.classList.add("popup-table")
    container.appendChild(table);

    /** @private
     *  @type {HTMLDivElement}
     */
    this.rootElem = container;
    /** @private
     *  @type {HTMLParagraphElement}
     */
    this.titleElem = title;
    /** @private
     * @type {HTMLTableCellElement}
     */
    this.regionElem = dataCells[0];
    /** @private
     * @type {HTMLTableCellElement}
     */
    this.populationElem = dataCells[1];

    this.setVisible(false);
    this.setPos(0, 0);
  }

  /** 
   *  @param {number} x
   *  @param {number} y
   *  @param {Area} data
   */
  show(data, x, y) {
    this.setData(data);
    this.setPos(x, y);
    this.setVisible(true);
  }

  hide() {
    this.setVisible(false);
  }

  /**
   * @param {Area} data
   */
  setData(data) {
    this.titleElem.innerText = data.name;
    this.populationElem.innerText = data.totalPopulation;
    this.regionElem.innerText = data.historicalRegion;
  }

  /**
   * @private
   * @param {boolean} val
   */
  setVisible(val) {
    this.rootElem.style.display = val ? "block" : "none";
    this.rootElem.style.opacity = val ? "90%" : "0";
  }

  /**
   * @private
   * @param {number} x
   * @param {number} y
   */
  setPos(x, y) {
    const offset = 10;

    this.rootElem.style.left = `${offset + x}px`;
    this.rootElem.style.top = `${offset + y}px`;
  }
}

class PieChart {
  /**
   * @param {ChartItem[]} data 
   */
  constructor(data) {
    /**
     * @private
     * @type {ChartItem[]}
     */
    this.data = data;
    for (let i = 0; i < this.data.length; i++) {
      if (data[i].value == 0) {
        this.data.splice(i, 1);
      }
    }

    /**
     * @private
     * @type {HTMLUListElement}
     */
    this.listElement = this.createList();

    /**
     * @private
     * @type {SVGSVGElement}
     */
    this.svgElement = this.createSVG();

    /**
     * @private
     * @type {HTMLDivElement}
     */
    this.element = document.createElement("div");
    this.element.classList.add("pie-chart-container");
    this.element.appendChild(this.listElement);
    this.element.appendChild(this.svgElement);
  }

  /**
   * @param {ChartItem[]} data 
   */
  setData(data) {
    this.data = data;
    for (let i = 0; i < this.data.length; i++) {
      if (data[i].value == 0) {
        this.data.splice(i, 1);
      }
    }

    this.listElement.remove();
    this.listElement = null;
    this.listElement = this.createList();
    this.element.appendChild(this.listElement);

    this.svgElement.remove();
    this.svgElement = null;
    this.svgElement = this.createSVG();
    this.element.appendChild(this.svgElement);
  }

  getElement() {
    return this.element;
  }

  /**
   * @private
   * @returns {HTMLUListElement}
   */
  createList() {
    const listElement = document.createElement("ul");
    listElement.classList.add("pie-chart-list");

    const total = this.getTotal();
    for (const elem of this.data) {
      const percentage = getPercentage(elem.value, total);
      const listItem = document.createElement("li");
      listItem.classList.add("pie-chart-list-item");
      listItem.innerHTML = `
      <span>
        <span class="pie-chart-list-item-color" style="background-color: ${elem.color}"></span> 
        ${elem.label} 
      </span>
      <span>${percentage}%<span>`;
      listElement.appendChild(listItem);
    }

    return listElement;
  }

  /**
   * @private
   * @returns {SVGSVGElement}
   */
  createSVG() {
    const svgElement = document.createElementNS("http://www.w3.org/2000/svg", "svg");

    svgElement.setAttribute("viewBox", "-1 -1 2 2");
    svgElement.style.transform = "rotate(-90deg)";
    svgElement.classList.add("pie-chart");
    svgElement.append(...this.createPaths());

    return svgElement;
  }

  /**
   * @private
   * @returns {SVGPathElement[]}
   */
  createPaths() {
    /** @type {SVGPathElement[]} */
    const out = [];
    const total = this.getTotal();

    let cumulativePercent = 0;
    for (const item of this.data) {
      const { x: startX, y: startY } = this.getCoordinatesForPercent(cumulativePercent);
      cumulativePercent += item.value / total;
      const { x: endX, y: endY } = this.getCoordinatesForPercent(cumulativePercent);

      const isLargeArc = (item.value / total) > 0.5 ? 1 : 0;
      const pathData = [
        `M ${startX} ${startY}`, // Move
        `A 1 1 0 ${isLargeArc} 1 ${endX} ${endY}`, // Arc
        `L 0 0`, // Line        
      ].join(" ");

      const pathElement = document.createElementNS("http://www.w3.org/2000/svg", "path");
      pathElement.setAttribute("d", pathData);
      pathElement.setAttribute("fill", item.color);
      out.push(pathElement);
    }

    return out;
  }

  /**
   * @private
   * @returns {number}
   */
  getTotal() {
    if (this.data.length == 0) {
      return 0;
    }

    return this.data.reduce((acc, prev) => acc + prev.value, 0);
  }

  /**
   * @private
   * @param {number} percent 
   */
  getCoordinatesForPercent(percent) {
    return { x: Math.cos(2 * Math.PI * percent), y: Math.sin(2 * Math.PI * percent) };
  }
}


/** @type { HTMLDivElement } */
const mapContainer = querySelectorOrThrow(".map-container");
/** @type { SVGSVGElement } */
const mapSvg = querySelectorOrThrow("svg.map");
/** @type { NodeListOf<SVGPathElement> } */
const areas = querySelectorAllOrThrow(".region");
/** @type { HTMLDivElement } */
const statisticsContainer = querySelectorOrThrow(".statistics-container");
/** @type { HTMLParagraphElement } */
const statisticsTitle = querySelectorOrThrow(".statistics-title");

const popup = new Popup();

const ETHNICITIES_COLORS = ["#d30935", "#fc4111", "#17b1f3", "#68cb25", "#f8e930", "#f8b330", "#0ea145", "#0c90a7", "#00d3bd", "#fb8c1a", "#037aff", "#0b2fdd", "#5b00af", "#a60193", "#fe4ad3", "#f9b6e9", "#eae9e5", "#6f7c9a", "#031133", "#770124", "#874501", "#fcab6b", "#1c0fbe", "#fcd281",];
const pieChart = new PieChart(convertEthnicitiesToChartValues(AREAS.RO.ethnicities));
statisticsContainer.appendChild(pieChart.getElement());

/** @type { SVGPathElement | null} */
let selectedArea = null;

// Set up the map
for (const area of areas) {
  area.addEventListener("click", () => {
    selectArea(area);
  })
  area.addEventListener("mouseover", e => {
    popup.show(getAreaData(area), e.pageX, e.pageY);
  });
  area.addEventListener("mouseleave", () => {
    popup.hide();
  });
}

// Utility functions
/**
 * @param {SVGPathElement} area
 */
function selectArea(area) {
  if (selectedArea == area) {
    return;
  } else if (selectedArea != null) {
    selectedArea.classList.remove("region-selected");
  }

  selectedArea = area;
  area.classList.add("region-selected");

  const data = getAreaData(area);
  if (!data) {
    throw new Error("Data not found");
  }
  statisticsTitle.innerText = `Statistici ${data.name}`;
  pieChart.setData(convertEthnicitiesToChartValues(data.ethnicities));
}

/**
 * @param {Element} elem
 * @returns {Area | null}
 */
function getAreaData(elem) {
  const id = getAttribOrThrow(elem, "id");
  if (id in AREAS) {
    return AREAS[id];
  } else {
    return null;
  }
}

/**
 * @param {Ethnicity[]} ethnicities  
 * @returns {ChartItem[]}
 */
function convertEthnicitiesToChartValues(ethnicities) {
  /** @type {ChartItem[]} */
  const out = [];
  const OTHER_ETHNICITIES_IDX = 22;

  let total = ethnicities[0].total;

  for (let i = 1; i < ethnicities.length; i++) {
    const ethnicity = ethnicities[i];
    const percent = ethnicity.total / total;

    if (percent < 0.001 && i != OTHER_ETHNICITIES_IDX) {
      ethnicities[OTHER_ETHNICITIES_IDX].total += ethnicity.total;
      continue;
    }

    let color = "white";
    if (ethnicity.type < ETHNICITIES_COLORS.length) {
      color = ETHNICITIES_COLORS[ethnicity.type];
    }
    out.push({ label: ethnicity.name, value: ethnicity.total, color });
  }
  return out;
}

/**
 * @param {string[]} rowNames 
 */
function createTable(rowNames) {
  const table = document.createElement("table");
  const body = document.createElement("tbody");
  table.appendChild(body);

  /** @type {HTMLTableCellElement[]} */
  const dataCells = [];

  for (const name of rowNames) {
    const row = document.createElement("tr");

    const nameCell = document.createElement("td");
    nameCell.innerText = name;
    row.appendChild(nameCell);

    const dataCell = document.createElement("td");
    dataCell.innerText = "...";
    row.appendChild(dataCell);
    dataCells.push(dataCell);

    body.appendChild(row);
  }

  return { table, dataCells };
}

/**
 * @param {string} selector
 * @throws if no element was found
 * @returns {Element}
 */
function querySelectorOrThrow(selector) {
  const elem = document.querySelector(selector);
  if (!elem) {
    throw new Error(`No element was found for ${selector}`);
  }

  return elem;
}

/**
 * @param {string} selector
 * @throws if no element was found
 * @returns {NodeListOf.<Element>}
 */
function querySelectorAllOrThrow(selector) {
  const elems = document.querySelectorAll(selector);
  if (elems.length == 0) {
    throw new Error(`No elements were found for ${selector}`);
  }

  return elems;
}

/**
 * @param {Element} element
 * @param {string} name
 * @returns {string}
 * @throws if the attribute was not found
 */
function getAttribOrThrow(element, name) {
  const attrib = element.getAttribute(name);
  if (!attrib) {
    throw new Error(`Attribute ${name} not found in ${element.tagName}`);
  }

  return attrib;
}

/**
 * @param {number} val 
 * @param {number} total 
 * @param {number} nrOfDecimlas
 * @returns {number}
 */
function getPercentage(val, total, nrOfDecimlas = 2) {
  const num = (val / total * 100).toFixed(nrOfDecimlas);
  return Number(num);
}
