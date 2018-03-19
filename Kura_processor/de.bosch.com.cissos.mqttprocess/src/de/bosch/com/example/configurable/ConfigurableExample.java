package de.bosch.com.example.configurable;

import java.net.URL;
import java.util.Iterator;
import java.util.Map;
import java.util.Map.Entry;

import org.apache.camel.CamelContext;
import org.apache.camel.builder.RouteBuilder;
import org.apache.camel.core.osgi.OsgiDefaultCamelContext;
import org.eclipse.kura.configuration.ConfigurableComponent;
import org.osgi.framework.BundleContext;
import org.osgi.service.component.ComponentContext;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class ConfigurableExample implements ConfigurableComponent {
	
	private static final Logger s_logger = LoggerFactory.getLogger(ConfigurableExample.class);
	private static final String APP_ID = "de.bosch.com.example.configurable.ConfigurableExample";
	private Map<String, Object> properties;
	private CamelContext ctx;
	private URL specUrl;
	private String urlPath;
	private String source = "+/+/+/+/+/stream";
	private String target = "BCDS.mqtt.transformed.stream";
	private ComponentContext componentContext;
	
	//on startup
	protected void activate(ComponentContext componentContext) {
		this.componentContext = componentContext;
		//configure a new camel route
		configureCamelRoute();
		try{
			ctx.start();
		}
		catch(Exception e) {
			e.printStackTrace();
		}
		s_logger.info("Bundle " + APP_ID + " has started!");
	}
	
	//on activate with properties
	protected void activate(ComponentContext componentContext, Map<String, Object> properties) {
		this.componentContext = componentContext;
		updated(properties);
		configureCamelRoute();
		try{
			ctx.start();
		}
		catch(Exception e) {
			e.printStackTrace();
		}
		s_logger.info("Bundle " + APP_ID + "has started with config!");
	}
	
	protected void deactivate(ComponentContext componentContext) {
        s_logger.info("Bundle " + APP_ID + " has stopped!");
        try {
        	ctx.stop();
        }
        catch(Exception e) {
        	e.printStackTrace();
        }
    }
	//on update
	public void updated(Map<String, Object> properties) {
		this.properties = properties;
		if(properties != null && !properties.isEmpty()) {
			Iterator<Entry<String, Object>> it = properties.entrySet().iterator();
			while(it.hasNext()){
				Entry<String, Object> entry = it.next();
				s_logger.info("New Property - " + entry.getKey() + " = " + entry.getValue() + 
						"of type " + entry.getValue().getClass().toString());
			}
		}
		try {
			//stop the camel context
			ctx.stop();
			//configure the context again using the new parameters
			configureCamelRoute();
			//start the context again
			ctx.start();
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	

	//configuring the camel route
	public void configureCamelRoute() {
		//instantiate the Camel Context
		BundleContext bundleContext = componentContext.getBundleContext();
		ctx = new OsgiDefaultCamelContext(bundleContext);
		//read the current properties for source and target and save to the variables
		if(properties != null && !properties.isEmpty()) {
			Iterator<Entry<String, Object>> it = properties.entrySet().iterator();
			while(it.hasNext()){
				Entry<String, Object> entry = it.next();
				if(entry.getKey().equals("sourceChannel.string")) {source = entry.getValue().toString();}
				if(entry.getKey().equals("targetChannel.string")) {target = entry.getValue().toString();}
			}
		}
		s_logger.info(source);
		s_logger.info(target);
		try {
			//configure the new routes
			ctx.addRoutes(new RouteBuilder() {
				public void configure() {

	            	//subscribing to the broker under the topic specified in the properties
	            	from("mqtt://incoming?host=tcp://localhost:1883&subscribeTopicName="+source)
                    //transformation using the Camel JOLT component according to the specified spec.json
	            	.to("jolt:resources/spec.json?inputType=JsonString&outputType=JsonString")
	            	//publishing the data back to the broker under the topic specified in the properties
                    .to("mqtt:sender?host=tcp://localhost:1883&publishTopicName="+target);
				}
			});			
		}
		catch (Exception e) {
			e.printStackTrace();
		}
	}
}
